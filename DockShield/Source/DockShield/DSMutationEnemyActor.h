#pragma once

#include "CoreMinimal.h"
#include "DSTargetActor.h"
#include "DSMutationEnemyActor.generated.h"

UENUM(BlueprintType)
enum class EDSMutationThreatState : uint8
{
    Dormant UMETA(DisplayName = "Dormant"),
    Alert UMETA(DisplayName = "Alert"),
    Staggered UMETA(DisplayName = "Staggered"),
    Bound UMETA(DisplayName = "Bound"),
    Defeated UMETA(DisplayName = "Defeated")
};

UCLASS(Blueprintable)
class DOCKSHIELD_API ADSMutationEnemyActor : public ADSTargetActor
{
    GENERATED_BODY()

public:
    ADSMutationEnemyActor();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Mutation")
    bool ApplyReelImpact(float Strength = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Mutation")
    bool ApplyFlyPressure(float Strength = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Mutation")
    bool ApplyLillyPressure(float Strength = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Mutation")
    bool EvaluateHeroCombo();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Mutation")
    void ResetMutationEncounter();

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    FString GetMutationStatusText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    FString GetThreatStateText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    EDSMutationThreatState GetThreatState() const { return ThreatState; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    float GetAggression() const { return Aggression; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    float GetVulnerability() const { return Vulnerability; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    int32 GetReelImpactCount() const { return ReelImpactCount; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    int32 GetFlyPressureCount() const { return FlyPressureCount; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    int32 GetLillyPressureCount() const { return LillyPressureCount; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    int32 GetComboImpactCount() const { return ComboImpactCount; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    bool IsDefeated() const { return ThreatState == EDSMutationThreatState::Defeated; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Mutation")
    FString MutationName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Mutation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ComboDamage;

private:
    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    EDSMutationThreatState ThreatState = EDSMutationThreatState::Dormant;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    float Health = 1.0f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    float Aggression = 0.35f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    float Stagger = 0.0f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    float Vulnerability = 0.0f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    int32 ReelImpactCount = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    int32 FlyPressureCount = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    int32 LillyPressureCount = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    int32 ComboImpactCount = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    int32 LastResolvedComboSignature = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation")
    FString LastCombatText = TEXT("NO CONTACT");

    void ApplyDamage(float DamageAmount, const FString& SourceText);
    int32 GetCurrentComboSignature() const;
};
