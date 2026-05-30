#include "DSTargetableComponent.h"
#include "GameFramework/Actor.h"

UDSTargetableComponent::UDSTargetableComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    DisplayName = FText::FromString(TEXT("DockShield Target"));
}

void UDSTargetableComponent::BeginPlay()
{
    Super::BeginPlay();
    ConfigureFromOwnerTags();
}

void UDSTargetableComponent::ConfigureFromOwnerTags()
{
    const AActor* Owner = GetOwner();
    if (!Owner)
    {
        return;
    }

    bCanReelPull = false;
    bCanFlyMark = false;
    bCanLillyBind = false;

    if (Owner->ActorHasTag(TEXT("LegendaryDuct")) || Owner->ActorHasTag(TEXT("Duct")))
    {
        TargetType = EDSTargetType::LegendaryDuct;
        DisplayName = FText::FromString(TEXT("Duct - Legendary Rubber Duck"));
        bCanReelPull = true;
        bCanFlyMark = true;
        bCanLillyBind = true;
    }
    else if (Owner->ActorHasTag(TEXT("GrapplePoint")))
    {
        TargetType = EDSTargetType::GrapplePoint;
        DisplayName = FText::FromString(TEXT("Grapple Pull Target"));
        bCanReelPull = true;
        bCanFlyMark = true;
    }
    else if (Owner->ActorHasTag(TEXT("Civilian")))
    {
        TargetType = EDSTargetType::Civilian;
        DisplayName = FText::FromString(TEXT("Civilian Rescue Target"));
        bCanReelPull = true;
        bCanFlyMark = true;
        bCanLillyBind = true;
    }
    else if (Owner->ActorHasTag(TEXT("Hazard")))
    {
        TargetType = EDSTargetType::Hazard;
        DisplayName = FText::FromString(TEXT("Toxic Hazard"));
        bCanReelPull = false;
        bCanFlyMark = true;
        bCanLillyBind = true;
    }
    else if (Owner->ActorHasTag(TEXT("Boat")))
    {
        TargetType = EDSTargetType::Boat;
        DisplayName = FText::FromString(TEXT("Prototype Rescue Boat"));
        bCanReelPull = true;
        bCanFlyMark = true;
    }
    else if (Owner->ActorHasTag(TEXT("MutationEnemy")))
    {
        TargetType = EDSTargetType::Enemy;
        DisplayName = FText::FromString(TEXT("Bayou Mutation Bruiser"));
        bCanReelPull = true;
        bCanFlyMark = true;
        bCanLillyBind = true;
    }
    else if (Owner->ActorHasTag(TEXT("Enemy")))
    {
        TargetType = EDSTargetType::Enemy;
        DisplayName = FText::FromString(TEXT("Enemy Patrol"));
        bCanFlyMark = true;
        bCanLillyBind = true;
    }
    else if (Owner->ActorHasTag(TEXT("Boss")))
    {
        TargetType = EDSTargetType::Boss;
        DisplayName = FText::FromString(TEXT("Deep Dock Mutation"));
        bCanFlyMark = true;
        bCanLillyBind = true;
    }
    else if (Owner->ActorHasTag(TEXT("WeakPoint")))
    {
        TargetType = EDSTargetType::WeakPoint;
        DisplayName = FText::FromString(TEXT("Structural Weak Point"));
        bCanReelPull = true;
        bCanFlyMark = true;
        bCanLillyBind = true;
    }
    else if (Owner->ActorHasTag(TEXT("Object")))
    {
        TargetType = EDSTargetType::Object;
        DisplayName = FText::FromString(TEXT("Interactable Object"));
        bCanReelPull = true;
        bCanFlyMark = true;
        bCanLillyBind = true;
    }
}

FString UDSTargetableComponent::GetReelPrompt() const
{
    if (!bCanReelPull)
    {
        return FString::Printf(TEXT("%s: no Reel action"), *DisplayName.ToString());
    }

    switch (TargetType)
    {
    case EDSTargetType::GrapplePoint:
        return FString::Printf(TEXT("Press E: Grapple Pull to %s"), *DisplayName.ToString());
    case EDSTargetType::Civilian:
        return FString::Printf(TEXT("Press E: Rescue Reel %s"), *DisplayName.ToString());
    case EDSTargetType::Boat:
        return FString::Printf(TEXT("Press E: Tow / Reel %s"), *DisplayName.ToString());
    case EDSTargetType::WeakPoint:
        return bIsReelExposed
            ? FString::Printf(TEXT("REEL EXPOSED: %s"), *GetComboStateText())
            : FString::Printf(TEXT("Press E: Reel expose %s"), *DisplayName.ToString());
    case EDSTargetType::Enemy:
        return FString::Printf(TEXT("Press E/LMB: Reel-stagger %s"), *DisplayName.ToString());
    case EDSTargetType::LegendaryDuct:
        return TEXT("DUCT SIGHTING: LMB/E latch, hold R; capture protocol impossible");
    default:
        return FString::Printf(TEXT("Press E: Reel Pull %s"), *DisplayName.ToString());
    }
}

bool UDSTargetableComponent::ExposeForReel(float Strength)
{
    ConfigureFromOwnerTags();
    if (!bCanReelPull)
    {
        return false;
    }

    bIsReelExposed = true;
    ReelExposureStrength = FMath::Clamp(Strength, 0.0f, 1.0f);
    ++ReelExposureCount;
    return true;
}

bool UDSTargetableComponent::CanReelPull() const
{
    return bCanReelPull;
}

void UDSTargetableComponent::ClearReelExposure()
{
    bIsReelExposed = false;
    ReelExposureStrength = 0.0f;
}

bool UDSTargetableComponent::IsReelExposed() const
{
    return bIsReelExposed;
}

bool UDSTargetableComponent::MarkForFly(float Strength)
{
    ConfigureFromOwnerTags();
    if (!bCanFlyMark)
    {
        return false;
    }

    bIsFlyMarked = true;
    FlyMarkStrength = FMath::Clamp(Strength, 0.0f, 1.0f);
    ++FlyMarkCount;
    return true;
}

void UDSTargetableComponent::ClearFlyMark()
{
    bIsFlyMarked = false;
    FlyMarkStrength = 0.0f;
}

bool UDSTargetableComponent::CanFlyMark() const
{
    return bCanFlyMark;
}

bool UDSTargetableComponent::IsFlyMarked() const
{
    return bIsFlyMarked;
}

FString UDSTargetableComponent::GetFlyPrompt() const
{
    if (!bCanFlyMark)
    {
        return FString::Printf(TEXT("%s: no Fly mark"), *DisplayName.ToString());
    }

    const FString MarkState = bIsFlyMarked ? TEXT("MARKED") : TEXT("UNMARKED");
    switch (TargetType)
    {
    case EDSTargetType::Hazard:
        return FString::Printf(TEXT("%s: Sonar tag toxic hazard"), *MarkState);
    case EDSTargetType::Enemy:
        return FString::Printf(TEXT("%s: Mark enemy patrol"), *MarkState);
    case EDSTargetType::Boss:
        return FString::Printf(TEXT("%s: Mark boss core"), *MarkState);
    case EDSTargetType::WeakPoint:
        return FString::Printf(TEXT("%s: Mark weak point"), *MarkState);
    case EDSTargetType::Civilian:
        return FString::Printf(TEXT("%s: Locate rescue target"), *MarkState);
    case EDSTargetType::Boat:
        return FString::Printf(TEXT("%s: Track extraction boat"), *MarkState);
    case EDSTargetType::LegendaryDuct:
        return FString::Printf(TEXT("%s: Track Duct wake signature"), *MarkState);
    default:
        return FString::Printf(TEXT("%s: Fly Sense mark %s"), *MarkState, *DisplayName.ToString());
    }
}

bool UDSTargetableComponent::BindForLilly(float Strength)
{
    ConfigureFromOwnerTags();
    if (!bCanLillyBind)
    {
        return false;
    }

    bIsLillyBound = true;
    LillyBindStrength = FMath::Clamp(Strength, 0.0f, 1.0f);
    ++LillyBindCount;
    return true;
}

void UDSTargetableComponent::ClearLillyBind()
{
    bIsLillyBound = false;
    LillyBindStrength = 0.0f;
}

bool UDSTargetableComponent::CanLillyBind() const
{
    return bCanLillyBind;
}

bool UDSTargetableComponent::IsLillyBound() const
{
    return bIsLillyBound;
}

FString UDSTargetableComponent::GetLillyPrompt() const
{
    if (!bCanLillyBind)
    {
        return FString::Printf(TEXT("%s: no Lilly bind"), *DisplayName.ToString());
    }

    const FString BindState = bIsLillyBound ? TEXT("BOUND") : TEXT("UNBOUND");
    switch (TargetType)
    {
    case EDSTargetType::Hazard:
        return FString::Printf(TEXT("%s: Root-suppress toxic hazard"), *BindState);
    case EDSTargetType::Enemy:
        return FString::Printf(TEXT("%s: Root snare enemy patrol"), *BindState);
    case EDSTargetType::Boss:
        return FString::Printf(TEXT("%s: Root-pressure boss core"), *BindState);
    case EDSTargetType::WeakPoint:
        return FString::Printf(TEXT("%s: Bind structural weak point"), *BindState);
    case EDSTargetType::Civilian:
        return FString::Printf(TEXT("%s: Stabilize rescue target"), *BindState);
    case EDSTargetType::Object:
        return FString::Printf(TEXT("%s: Vine shift object"), *BindState);
    case EDSTargetType::LegendaryDuct:
        return FString::Printf(TEXT("%s: Root-read Duct ripple"), *BindState);
    default:
        return FString::Printf(TEXT("%s: Lilly Bind %s"), *BindState, *DisplayName.ToString());
    }
}

bool UDSTargetableComponent::IsHookLineSinkerReady() const
{
    return bIsReelExposed && bIsFlyMarked && bIsLillyBound;
}

FString UDSTargetableComponent::GetComboStateText() const
{
    return FString::Printf(
        TEXT("REEL %s | FLY %s | LILLY %s"),
        bIsReelExposed ? TEXT("EXPOSED") : TEXT("NEEDED"),
        bIsFlyMarked ? TEXT("MARKED") : TEXT("NEEDED"),
        bIsLillyBound ? TEXT("BOUND") : TEXT("NEEDED"));
}
