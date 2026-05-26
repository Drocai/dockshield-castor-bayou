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
    Occupant = Rider;

    Rider->SetActorLocation(GetSeatWorldLocation(), false, nullptr, ETeleportType::TeleportPhysics);
    Rider->SetActorRotation(GetActorRotation(), ETeleportType::TeleportPhysics);
    Rider->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
    return true;
}

void ADSPrototypeBoatActor::ExitBoat()
{
    if (AActor* Rider = Occupant.Get())
    {
        Rider->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Rider->SetActorLocation(GetExitWorldLocation(), false, nullptr, ETeleportType::TeleportPhysics);
    }

    Occupant.Reset();
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

bool ADSPrototypeBoatActor::ApplyPilotInput(FVector2D MoveInput, float ControllerYawDegrees, float DeltaSeconds)
{
    LastPilotDistance = 0.0f;

    if (!bOccupied || bAnchored || !CanFloat() || DeltaSeconds <= 0.0f)
    {
        return false;
    }

    MoveInput = MoveInput.GetClampedToMaxSize(1.0f);
    if (MoveInput.IsNearlyZero())
    {
        return false;
    }

    const FRotator YawRotation(0.0f, ControllerYawDegrees, 0.0f);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    FVector PilotDirection = (ForwardDirection * MoveInput.Y) + (RightDirection * MoveInput.X);
    PilotDirection.Z = 0.0f;
    if (!PilotDirection.Normalize())
    {
        return false;
    }

    const float DepthAssist = FMath::Clamp(CurrentWaterDepth / (RequiredWaterDepth * 2.0f), 0.35f, 1.0f);
    LastPilotDistance = FMath::Max(0.0f, PilotSpeed) * DepthAssist * DeltaSeconds * MoveInput.Size();
    FVector NewLocation = GetActorLocation() + (PilotDirection * LastPilotDistance);
    NewLocation.Z = BaseZ;

    SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
    SetActorRotation(PilotDirection.Rotation(), ETeleportType::TeleportPhysics);
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

float ADSPrototypeBoatActor::GetLastPilotDistance() const
{
    return LastPilotDistance;
}

FVector ADSPrototypeBoatActor::GetSeatWorldLocation() const
{
    return GetActorTransform().TransformPosition(SeatOffset);
}

FVector ADSPrototypeBoatActor::GetExitWorldLocation() const
{
    return GetActorTransform().TransformPosition(ExitOffset);
}
