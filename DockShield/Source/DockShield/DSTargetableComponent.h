#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSTargetTypes.h"
#include "DSTargetableComponent.generated.h"

UCLASS(ClassGroup = (DockShield), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class DOCKSHIELD_API UDSTargetableComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDSTargetableComponent();

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Targeting")
    EDSTargetType TargetType = EDSTargetType::Unknown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Targeting")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Targeting")
    float InteractionRange = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Targeting")
    bool bCanReelPull = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Targeting")
    bool bCanFlyMark = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Targeting")
    bool bCanLillyBind = false;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Targeting")
    void ConfigureFromOwnerTags();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Targeting")
    FString GetReelPrompt() const;
};
