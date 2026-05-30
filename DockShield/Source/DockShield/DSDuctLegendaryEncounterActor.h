#pragma once

#include "CoreMinimal.h"
#include "DSTargetActor.h"
#include "DSDuctLegendaryEncounterActor.generated.h"

UENUM(BlueprintType)
enum class EDSDuctEncounterState : uint8
{
    Hidden UMETA(DisplayName = "Hidden"),
    Sighted UMETA(DisplayName = "Sighted"),
    Lured UMETA(DisplayName = "Lured"),
    Latched UMETA(DisplayName = "Latched"),
    NearCatch UMETA(DisplayName = "Near Catch"),
    Escaped UMETA(DisplayName = "Escaped")
};

UCLASS(Blueprintable)
class DOCKSHIELD_API ADSDuctLegendaryEncounterActor : public ADSTargetActor
{
    GENERATED_BODY()

public:
    ADSDuctLegendaryEncounterActor();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Duct")
    bool BeginSighting();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Duct")
    bool ApplyBait(FName BaitId);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Duct")
    bool EvaluateRareSpawnForRoll(float Roll, float WeatherHazardScale, float MissionProgress, int32 BaitTier);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Duct")
    bool LatchWithReel(float InitialTension = 0.0f);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Duct")
    bool AdvanceReelBattle(float DeltaSeconds, float LineTension);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Duct")
    void ResolveSlipAway();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Duct")
    void ResetDuctEncounter();

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    FString GetDuctStatusText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    FString GetRarityStatusText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    FString GetBaitEconomyStatusText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    FString GetEncounterStateText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    float CalculateSpawnChance(float WeatherHazardScale, float MissionProgress, int32 BaitTier) const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    EDSDuctEncounterState GetEncounterState() const { return EncounterState; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    bool IsCatchable() const { return false; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    bool HasSlippedAway() const { return EncounterState == EDSDuctEncounterState::Escaped; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    float GetEncounterProgress() const { return EncounterProgress; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    float GetBestNearCatchProgress() const { return BestNearCatchProgress; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    int32 GetSightingCount() const { return SightingCount; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    int32 GetAttemptCount() const { return AttemptCount; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    int32 GetBaitAttemptCount() const { return BaitAttemptCount; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    FName GetLastBaitId() const { return LastBaitId; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    float GetLastSpawnChance() const { return LastSpawnChance; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    float GetLastSpawnRoll() const { return LastSpawnRoll; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    int32 GetNearCatchCount() const { return NearCatchCount; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Duct")
    int32 GetDuctTapeTraceCount() const { return DuctTapeTraceCount; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Duct", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float NearCatchThreshold = 0.86f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Duct", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SlipAwayThreshold = 0.985f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Duct", meta = (ClampMin = "0.0"))
    float BaseProgressRate = 0.22f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Duct", meta = (ClampMin = "0.0"))
    float TensionProgressRate = 0.30f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Duct|Rarity", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BaseRareSpawnChance = 0.025f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Duct|Rarity", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float WeatherHazardSpawnBonus = 0.035f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Duct|Rarity", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MissionProgressSpawnBonus = 0.06f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Duct|Rarity", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BaitTierSpawnBonus = 0.045f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Duct|Rarity", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MaxRareSpawnChance = 0.18f;

private:
    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    EDSDuctEncounterState EncounterState = EDSDuctEncounterState::Hidden;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    float EncounterProgress = 0.0f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    float BestNearCatchProgress = 0.0f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    int32 SightingCount = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    int32 BaitAttemptCount = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    FName LastBaitId = FName(TEXT("None"));

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    int32 LastBaitTier = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    int32 LastBaitCost = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    float LastSpawnChance = 0.0f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    float LastSpawnRoll = 1.0f;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    int32 AttemptCount = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    int32 NearCatchCount = 0;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Duct")
    int32 DuctTapeTraceCount = 0;

    bool bNearCatchRecordedThisAttempt = false;
};
