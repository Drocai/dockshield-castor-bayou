#pragma once

#include "CoreMinimal.h"
#include "DSTargetActor.h"
#include "DSMutationEnemyActor.generated.h"

class UPointLightComponent;
class UStaticMeshComponent;

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
    virtual void Tick(float DeltaSeconds) override;

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
    FName GetLastFeedbackEvent() const { return LastFeedbackEvent; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    FString GetCombatFeedbackText() const { return LastFeedbackText; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    float GetFeedbackPulseRemaining() const { return FeedbackPulseRemaining; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    int32 GetFeedbackPulseCount() const { return FeedbackPulseCount; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Mutation")
    bool IsDefeated() const { return ThreatState == EDSMutationThreatState::Defeated; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Mutation Feedback")
    TObjectPtr<UPointLightComponent> FeedbackLight;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Mutation Feedback")
    TObjectPtr<UStaticMeshComponent> FeedbackRingMesh;

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

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation Feedback")
    FName LastFeedbackEvent = FName(TEXT("Ready"));

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation Feedback")
    FString LastFeedbackText = TEXT("COMBAT FEEDBACK READY");

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation Feedback")
    FLinearColor LastFeedbackColor = FLinearColor::Black;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation Feedback")
    float FeedbackPulseRemaining = 0.0f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation Feedback")
    float FeedbackPulseDuration = 0.0f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation Feedback")
    float FeedbackPulseIntensity = 0.0f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Mutation Feedback")
    int32 FeedbackPulseCount = 0;

    void ApplyDamage(float DamageAmount, const FString& SourceText);
    int32 GetCurrentComboSignature() const;
    void TriggerCombatFeedback(FName EventName, const FString& FeedbackText, const FLinearColor& FeedbackColor, float Intensity, float Duration);
};
