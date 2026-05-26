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

    if (Owner->ActorHasTag(TEXT("GrapplePoint")))
    {
        TargetType = EDSTargetType::GrapplePoint;
        DisplayName = FText::FromString(TEXT("Grapple Pull Target"));
        bCanReelPull = true;
    }
    else if (Owner->ActorHasTag(TEXT("Civilian")))
    {
        TargetType = EDSTargetType::Civilian;
        DisplayName = FText::FromString(TEXT("Civilian Rescue Target"));
        bCanReelPull = true;
    }
    else if (Owner->ActorHasTag(TEXT("Hazard")))
    {
        TargetType = EDSTargetType::Hazard;
        DisplayName = FText::FromString(TEXT("Toxic Hazard"));
        bCanReelPull = false;
    }
    else if (Owner->ActorHasTag(TEXT("Boat")))
    {
        TargetType = EDSTargetType::Boat;
        DisplayName = FText::FromString(TEXT("Prototype Rescue Boat"));
        bCanReelPull = true;
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
    default:
        return FString::Printf(TEXT("Press E: Reel Pull %s"), *DisplayName.ToString());
    }
}
