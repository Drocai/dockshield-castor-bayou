#include "DSWaterZoneActor.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

ADSWaterZoneActor::ADSWaterZoneActor()
{
    PrimaryActorTick.bCanEverTick = true;

    WaterBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("WaterBounds"));
    RootComponent = WaterBounds;
    WaterBounds->InitBoxExtent(FVector(1200.0f, 900.0f, 260.0f));
    WaterBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    WaterBounds->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ADSWaterZoneActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    AdvanceFlood(DeltaSeconds);

    if (!GetWorld() || !WaterBounds)
    {
        return;
    }

    const FColor WaterColor = bToxic ? FColor::Green : FColor::Blue;
    const FVector Center = GetActorLocation();
    const FVector Extent = WaterBounds->GetScaledBoxExtent();
    DrawDebugBox(GetWorld(), Center, Extent, WaterColor, false, 0.0f, 0, 1.5f);

    const FVector SurfaceCenter(Center.X, Center.Y, WaterSurfaceZ);
    const FVector CornerA = SurfaceCenter + FVector(Extent.X, Extent.Y, 0.0f);
    const FVector CornerB = SurfaceCenter + FVector(Extent.X, -Extent.Y, 0.0f);
    const FVector CornerC = SurfaceCenter + FVector(-Extent.X, -Extent.Y, 0.0f);
    const FVector CornerD = SurfaceCenter + FVector(-Extent.X, Extent.Y, 0.0f);
    DrawDebugLine(GetWorld(), CornerA, CornerB, WaterColor, false, 0.0f, 0, 3.0f);
    DrawDebugLine(GetWorld(), CornerB, CornerC, WaterColor, false, 0.0f, 0, 3.0f);
    DrawDebugLine(GetWorld(), CornerC, CornerD, WaterColor, false, 0.0f, 0, 3.0f);
    DrawDebugLine(GetWorld(), CornerD, CornerA, WaterColor, false, 0.0f, 0, 3.0f);

    const FVector CurrentVelocity = GetCurrentVelocity();
    if (!CurrentVelocity.IsNearlyZero())
    {
        const FVector ArrowStart = SurfaceCenter + FVector(0.0f, 0.0f, 28.0f);
        const FVector ArrowEnd = ArrowStart + CurrentVelocity.GetClampedToMaxSize(260.0f);
        DrawDebugDirectionalArrow(GetWorld(), ArrowStart, ArrowEnd, 90.0f, FColor::Cyan, false, 0.0f, 0, 3.0f);
    }
}

bool ADSWaterZoneActor::ContainsWorldLocation(FVector WorldLocation) const
{
    return WaterBounds && WaterBounds->Bounds.GetBox().IsInsideOrOn(WorldLocation);
}

float ADSWaterZoneActor::GetDepthAtLocation(FVector WorldLocation) const
{
    if (!ContainsWorldLocation(WorldLocation))
    {
        return 0.0f;
    }

    return FMath::Max(0.0f, WaterSurfaceZ - WorldLocation.Z);
}

float ADSWaterZoneActor::GetMovementScaleAtLocation(FVector WorldLocation) const
{
    const float Depth = GetDepthAtLocation(WorldLocation);
    float Scale = 1.0f;

    if (Depth > DeepWaterDepth)
    {
        Scale = 0.35f;
    }
    else if (Depth > WaistDeepDepth)
    {
        Scale = 0.58f;
    }
    else if (Depth > 1.0f)
    {
        Scale = 0.82f;
    }

    return bToxic ? Scale * 0.9f : Scale;
}

bool ADSWaterZoneActor::CanBoatOperateAtLocation(FVector WorldLocation) const
{
    return GetDepthAtLocation(WorldLocation) >= BoatOperationalDepth;
}

FVector ADSWaterZoneActor::GetCurrentVelocity() const
{
    return CurrentDirection.GetSafeNormal() * CurrentSpeed * (1.0f + FMath::Clamp(StormPressure, 0.0f, 1.0f));
}

FVector ADSWaterZoneActor::GetCurrentVelocityAtLocation(FVector WorldLocation) const
{
    const float Depth = GetDepthAtLocation(WorldLocation);
    if (Depth <= 1.0f)
    {
        return FVector::ZeroVector;
    }

    const float DepthScale = FMath::Clamp(Depth / FMath::Max(DeepWaterDepth, 1.0f), 0.25f, 1.0f);
    return GetCurrentVelocity() * DepthScale;
}

float ADSWaterZoneActor::GetFloodPressureAtLocation(FVector WorldLocation) const
{
    const float Depth = GetDepthAtLocation(WorldLocation);
    if (Depth <= 1.0f)
    {
        return 0.0f;
    }

    const float DepthPressure = FMath::Clamp(Depth / FMath::Max(DeepWaterDepth, 1.0f), 0.0f, 1.0f);
    const float CurrentPressure = FMath::Clamp(GetCurrentVelocityAtLocation(WorldLocation).Size() / 420.0f, 0.0f, 1.0f);
    const float ToxicPressure = bToxic ? ToxicPressureBonus : 0.0f;
    return FMath::Clamp((DepthPressure * 0.48f) + (CurrentPressure * 0.26f) + (StormPressure * 0.22f) + ToxicPressure, 0.0f, 1.0f);
}

float ADSWaterZoneActor::GetReelTensionPressureAtLocation(FVector WorldLocation) const
{
    const float MovementDrag = 1.0f - GetMovementScaleAtLocation(WorldLocation);
    return FMath::Clamp((MovementDrag * 0.58f) + (GetFloodPressureAtLocation(WorldLocation) * 0.62f), 0.0f, 1.0f);
}

void ADSWaterZoneActor::AdvanceFlood(float DeltaSeconds)
{
    if (DeltaSeconds <= 0.0f || FloodSurgeRate <= 0.0f)
    {
        return;
    }

    WaterSurfaceZ = FMath::Min(WaterSurfaceZ + (FloodSurgeRate * DeltaSeconds), MaxWaterSurfaceZ);
}

FString ADSWaterZoneActor::GetWaterStateTextAtLocation(FVector WorldLocation) const
{
    const float Pressure = GetFloodPressureAtLocation(WorldLocation);
    if (Pressure >= 0.72f)
    {
        return bToxic ? TEXT("TOXIC SURGE") : TEXT("FLOOD SURGE");
    }

    if (Pressure >= 0.42f)
    {
        return bToxic ? TEXT("TOXIC CURRENT") : TEXT("STRONG CURRENT");
    }

    if (GetDepthAtLocation(WorldLocation) > 1.0f)
    {
        return bToxic ? TEXT("TOXIC WATER") : TEXT("WATER");
    }

    return TEXT("DRY");
}
