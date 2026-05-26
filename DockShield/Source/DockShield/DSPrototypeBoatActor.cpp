#include "DSPrototypeBoatActor.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "DSTargetableComponent.h"
#include "UObject/ConstructorHelpers.h"

ADSPrototypeBoatActor::ADSPrototypeBoatActor()
{
    PrimaryActorTick.bCanEverTick = true;

    BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoatMesh"));
    RootComponent = BoatMesh;
    BoatMesh->SetWorldScale3D(FVector(2.8f, 1.2f, 0.3f));
    BoatMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoatMesh->SetCollisionResponseToAllChannels(ECR_Block);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        BoatMesh->SetStaticMesh(CubeMesh.Object);
    }

    TargetableComponent = CreateDefaultSubobject<UDSTargetableComponent>(TEXT("TargetableComponent"));

    Tags.AddUnique(TEXT("DockShieldTarget"));
    Tags.AddUnique(TEXT("Boat"));
}

void ADSPrototypeBoatActor::BeginPlay()
{
    Super::BeginPlay();
    BaseZ = GetActorLocation().Z;
}

void ADSPrototypeBoatActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    RunningTime += DeltaSeconds;
    if (CanFloat())
    {
        FVector Location = GetActorLocation();
        Location.Z = BaseZ + (FMath::Sin(RunningTime * BobSpeed) * BobAmplitude);
        SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
    }

    if (GetWorld())
    {
        const FColor DebugColor = CanFloat() ? FColor::Cyan : FColor::Orange;
        DrawDebugBox(GetWorld(), GetActorLocation(), FVector(145.0f, 65.0f, 28.0f), DebugColor, false, 0.0f, 0, 2.0f);
    }
}

void ADSPrototypeBoatActor::SetWaterDepth(float Depth)
{
    CurrentWaterDepth = FMath::Max(0.0f, Depth);
}

bool ADSPrototypeBoatActor::CanFloat() const
{
    return CurrentWaterDepth >= RequiredWaterDepth;
}

bool ADSPrototypeBoatActor::BoardBoat(AActor* Rider)
{
    if (!Rider || bOccupied || !CanFloat())
    {
        return false;
    }

    bOccupied = true;
    return true;
}

void ADSPrototypeBoatActor::ExitBoat()
{
    bOccupied = false;
}

bool ADSPrototypeBoatActor::ApplyReelTowFrom(FVector PullOrigin, float Strength)
{
    LastTowDistance = 0.0f;

    if (bAnchored || !CanFloat())
    {
        return false;
    }

    FVector TowDirection = PullOrigin - GetActorLocation();
    TowDirection.Z = 0.0f;
    if (!TowDirection.Normalize())
    {
        return false;
    }

    const float DepthAssist = FMath::Clamp(CurrentWaterDepth / (RequiredWaterDepth * 2.0f), 0.35f, 1.0f);
    LastTowDistance = FMath::Max(0.0f, Strength) * DepthAssist;
    FVector NewLocation = GetActorLocation() + (TowDirection * LastTowDistance);
    NewLocation.Z = BaseZ;
    SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
    BaseZ = NewLocation.Z;
    return true;
}

FString ADSPrototypeBoatActor::GetBoatStateText() const
{
    if (bAnchored)
    {
        return TEXT("ANCHORED");
    }

    if (!CanFloat())
    {
        return TEXT("TOO SHALLOW");
    }

    return bOccupied ? TEXT("OCCUPIED") : TEXT("READY");
}

float ADSPrototypeBoatActor::GetLastTowDistance() const
{
    return LastTowDistance;
}
