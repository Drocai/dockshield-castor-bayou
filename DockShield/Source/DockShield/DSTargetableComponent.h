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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Targeting")
    bool bIsFlyMarked = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Targeting")
    float FlyMarkStrength = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Targeting")
    int32 FlyMarkCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Targeting")
    bool bIsLillyBound = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Targeting")
    float LillyBindStrength = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Targeting")
    int32 LillyBindCount = 0;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Targeting")
    void ConfigureFromOwnerTags();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Targeting")
    FString GetReelPrompt() const;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Targeting")
    bool MarkForFly(float Strength = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Targeting")
    void ClearFlyMark();

    UFUNCTION(BlueprintPure, Category = "DockShield|Targeting")
    bool CanFlyMark() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Targeting")
    bool IsFlyMarked() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Targeting")
    FString GetFlyPrompt() const;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Targeting")
    bool BindForLilly(float Strength = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Targeting")
    void ClearLillyBind();

    UFUNCTION(BlueprintPure, Category = "DockShield|Targeting")
    bool CanLillyBind() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Targeting")
    bool IsLillyBound() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Targeting")
    FString GetLillyPrompt() const;
};
