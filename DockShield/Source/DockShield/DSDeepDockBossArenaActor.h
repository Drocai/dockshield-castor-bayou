#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectKey.h"
#include "DSDeepDockBossArenaActor.generated.h"

UENUM(BlueprintType)
enum class EDSDeepDockBossPhase : uint8
{
	Dormant UMETA(DisplayName = "Dormant"),
	Exposed UMETA(DisplayName = "Exposed"),
	Enraged UMETA(DisplayName = "Enraged"),
	Defeated UMETA(DisplayName = "Defeated")
};

UCLASS()
class DOCKSHIELD_API ADSDeepDockBossArenaActor : public AActor
{
	GENERATED_BODY()

public:
	ADSDeepDockBossArenaActor();

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	FString GetArenaStatusText() const;

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	float GetBossIntegrity() const { return BossIntegrity; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	float GetLabCoreIntegrity() const { return LabCoreIntegrity; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	float GetThreatLevel() const { return ThreatLevel; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	bool IsArenaArmed() const { return bArenaArmed; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	bool IsBossDefeated() const { return BossPhase == EDSDeepDockBossPhase::Defeated; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	EDSDeepDockBossPhase GetBossPhase() const { return BossPhase; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	FString GetBossPhaseText() const;

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	int32 GetResolvedWeakPointCount() const { return ResolvedWeakPointCount; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	int32 GetComboTriggerCount() const { return ComboTriggerCount; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	float GetHookLineSinkerReadiness() const;

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	float GetWeakPointComboReadiness(AActor* WeakPointActor) const;

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	int32 GetActiveComboWindowCount() const { return WeakPointComboWindows.Num(); }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	int32 GetExpiredComboWindowCount() const { return ExpiredComboWindowCount; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	FString GetComboWindowStatusText() const;

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	bool IsWeakPointWindowActive(AActor* WeakPointActor) const;

	UFUNCTION(BlueprintCallable, Category = "DockShield|Boss Arena")
	int32 EvaluateBossWeakPointCombos();

	UFUNCTION(BlueprintCallable, Category = "DockShield|Boss Arena")
	bool PrimeWeakPointDamageWindow(AActor* WeakPointActor);

	UFUNCTION(BlueprintCallable, Category = "DockShield|Boss Arena")
	void AdvanceBossWindowTimers(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "DockShield|Boss Arena")
	bool ApplyHookLineSinkerCombo(AActor* WeakPointActor);

	UFUNCTION(BlueprintCallable, Category = "DockShield|Boss Arena")
	void ResetBossEncounter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena")
	FString ArenaName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena")
	FString BossName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BossIntegrity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float LabCoreIntegrity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ThreatLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena")
	bool bArenaArmed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena", meta = (ClampMin = "1"))
	int32 WeakPointTargetCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float WeakPointDamagePerCombo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena", meta = (ClampMin = "0.5", ClampMax = "30.0"))
	float DefaultComboWindowSeconds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena")
	EDSDeepDockBossPhase BossPhase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena")
	int32 ResolvedWeakPointCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena")
	int32 ComboTriggerCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena")
	FString LastComboText;

private:
	TSet<FObjectKey> ResolvedWeakPoints;
	TMap<FObjectKey, float> WeakPointComboWindows;
	int32 ExpiredComboWindowCount;
	FString LastComboWindowText;

	bool IsBossWeakPointActor(const AActor* Actor) const;
	void UpdateBossPhaseFromState();
};
