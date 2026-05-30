#include "DSDeepDockBossArenaActor.h"
#include "DSTargetableComponent.h"
#include "EngineUtils.h"

ADSDeepDockBossArenaActor::ADSDeepDockBossArenaActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ArenaName = TEXT("DEEP DOCK BOSS ARENA");
	BossName = TEXT("DEEP DOCK MUTATION");
	BossIntegrity = 1.0f;
	LabCoreIntegrity = 0.72f;
	ThreatLevel = 0.85f;
	bArenaArmed = true;
	WeakPointTargetCount = 3;
	WeakPointDamagePerCombo = 0.34f;
	DefaultComboWindowSeconds = 6.0f;
	BossPhase = EDSDeepDockBossPhase::Dormant;
	ResolvedWeakPointCount = 0;
	ComboTriggerCount = 0;
	ExpiredComboWindowCount = 0;
	LastComboText = TEXT("NO COMBO");
	LastComboWindowText = TEXT("WINDOWS CLOSED");

	Tags.AddUnique(FName(TEXT("DockShieldBossArena")));
}

FString ADSDeepDockBossArenaActor::GetArenaStatusText() const
{
	const int32 BossPercent = FMath::RoundToInt(BossIntegrity * 100.0f);
	const int32 CorePercent = FMath::RoundToInt(LabCoreIntegrity * 100.0f);
	const int32 ThreatPercent = FMath::RoundToInt(ThreatLevel * 100.0f);

	return FString::Printf(
		TEXT("%s | PHASE %s | %s %d%% | CORE %d%% | THREAT %d%% | WEAK %d/%d | %s"),
		*ArenaName,
		*GetBossPhaseText(),
		*BossName,
		BossPercent,
		CorePercent,
		ThreatPercent,
		ResolvedWeakPointCount,
		WeakPointTargetCount,
		*LastComboText);
}

FString ADSDeepDockBossArenaActor::GetBossPhaseText() const
{
	switch (BossPhase)
	{
	case EDSDeepDockBossPhase::Exposed:
		return TEXT("EXPOSED");
	case EDSDeepDockBossPhase::Enraged:
		return TEXT("ENRAGED");
	case EDSDeepDockBossPhase::Defeated:
		return TEXT("DEFEATED");
	default:
		return TEXT("DORMANT");
	}
}

float ADSDeepDockBossArenaActor::GetHookLineSinkerReadiness() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return 0.0f;
	}

	float BestReadiness = 0.0f;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		const AActor* Actor = *It;
		if (!IsBossWeakPointActor(Actor))
		{
			continue;
		}

		const UDSTargetableComponent* Targetable = Actor->FindComponentByClass<UDSTargetableComponent>();
		if (!Targetable)
		{
			continue;
		}

		BestReadiness = FMath::Max(BestReadiness, GetWeakPointComboReadiness(const_cast<AActor*>(Actor)));
	}

	return FMath::Clamp(BestReadiness, 0.0f, 1.0f);
}

float ADSDeepDockBossArenaActor::GetWeakPointComboReadiness(AActor* WeakPointActor) const
{
	if (!IsBossWeakPointActor(WeakPointActor))
	{
		return 0.0f;
	}

	const UDSTargetableComponent* Targetable = WeakPointActor->FindComponentByClass<UDSTargetableComponent>();
	if (!Targetable)
	{
		return 0.0f;
	}

	float Readiness = 0.0f;
	Readiness += Targetable->IsReelExposed() ? 0.34f : 0.0f;
	Readiness += Targetable->IsFlyMarked() ? 0.33f : 0.0f;
	Readiness += Targetable->IsLillyBound() ? 0.33f : 0.0f;
	return FMath::Clamp(Readiness, 0.0f, 1.0f);
}

FString ADSDeepDockBossArenaActor::GetComboWindowStatusText() const
{
	return FString::Printf(
		TEXT("COMBO WINDOWS %d ACTIVE | EXPIRED %d | %s"),
		WeakPointComboWindows.Num(),
		ExpiredComboWindowCount,
		*LastComboWindowText);
}

bool ADSDeepDockBossArenaActor::IsWeakPointWindowActive(AActor* WeakPointActor) const
{
	if (!WeakPointActor)
	{
		return false;
	}

	const float* RemainingSeconds = WeakPointComboWindows.Find(FObjectKey(WeakPointActor));
	return RemainingSeconds && *RemainingSeconds > 0.0f;
}

int32 ADSDeepDockBossArenaActor::EvaluateBossWeakPointCombos()
{
	UWorld* World = GetWorld();
	if (!World || !bArenaArmed)
	{
		return 0;
	}

	AdvanceBossWindowTimers(0.5f);

	int32 AppliedComboCount = 0;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsBossWeakPointActor(Actor))
		{
			continue;
		}

		const float Readiness = GetWeakPointComboReadiness(Actor);
		if (Readiness >= 0.66f && Readiness < 1.0f)
		{
			PrimeWeakPointDamageWindow(Actor);
		}

		if (ApplyHookLineSinkerCombo(Actor))
		{
			++AppliedComboCount;
		}
	}

	return AppliedComboCount;
}

bool ADSDeepDockBossArenaActor::PrimeWeakPointDamageWindow(AActor* WeakPointActor)
{
	if (!IsBossWeakPointActor(WeakPointActor) || ResolvedWeakPoints.Contains(FObjectKey(WeakPointActor)))
	{
		return false;
	}

	const FObjectKey WeakPointKey(WeakPointActor);
	const float ExistingRemaining = WeakPointComboWindows.FindRef(WeakPointKey);
	if (ExistingRemaining > 0.0f)
	{
		return false;
	}

	WeakPointComboWindows.Add(WeakPointKey, DefaultComboWindowSeconds);
	LastComboWindowText = FString::Printf(TEXT("WINDOW OPEN %.1fs"), DefaultComboWindowSeconds);
	return true;
}

void ADSDeepDockBossArenaActor::AdvanceBossWindowTimers(float DeltaSeconds)
{
	if (DeltaSeconds <= 0.0f || WeakPointComboWindows.Num() == 0)
	{
		return;
	}

	TArray<FObjectKey> ExpiredKeys;
	for (TPair<FObjectKey, float>& WindowPair : WeakPointComboWindows)
	{
		WindowPair.Value -= DeltaSeconds;
		if (WindowPair.Value <= 0.0f)
		{
			ExpiredKeys.Add(WindowPair.Key);
		}
	}

	for (const FObjectKey& ExpiredKey : ExpiredKeys)
	{
		WeakPointComboWindows.Remove(ExpiredKey);
		++ExpiredComboWindowCount;
	}

	if (ExpiredKeys.Num() > 0)
	{
		LastComboWindowText = FString::Printf(TEXT("WINDOW EXPIRED x%d"), ExpiredComboWindowCount);
	}
}

bool ADSDeepDockBossArenaActor::ApplyHookLineSinkerCombo(AActor* WeakPointActor)
{
	if (!WeakPointActor || !bArenaArmed || ResolvedWeakPoints.Contains(FObjectKey(WeakPointActor)))
	{
		return false;
	}

	UDSTargetableComponent* Targetable = WeakPointActor->FindComponentByClass<UDSTargetableComponent>();
	if (!Targetable)
	{
		return false;
	}

	Targetable->ConfigureFromOwnerTags();
	if (!Targetable->IsHookLineSinkerReady())
	{
		return false;
	}

	if (!IsWeakPointWindowActive(WeakPointActor))
	{
		PrimeWeakPointDamageWindow(WeakPointActor);
	}

	ResolvedWeakPoints.Add(FObjectKey(WeakPointActor));
	WeakPointComboWindows.Remove(FObjectKey(WeakPointActor));
	ResolvedWeakPointCount = ResolvedWeakPoints.Num();
	++ComboTriggerCount;

	BossIntegrity = FMath::Clamp(BossIntegrity - WeakPointDamagePerCombo, 0.0f, 1.0f);
	LabCoreIntegrity = FMath::Clamp(LabCoreIntegrity - 0.08f, 0.0f, 1.0f);
	ThreatLevel = FMath::Clamp(ThreatLevel + 0.045f, 0.0f, 1.0f);
	LastComboText = FString::Printf(TEXT("HOOK, LINE & SINKER x%d"), ComboTriggerCount);
	LastComboWindowText = TEXT("WINDOW HIT");
	UpdateBossPhaseFromState();
	return true;
}

void ADSDeepDockBossArenaActor::ResetBossEncounter()
{
	BossIntegrity = 1.0f;
	LabCoreIntegrity = 0.72f;
	ThreatLevel = 0.85f;
	bArenaArmed = true;
	BossPhase = EDSDeepDockBossPhase::Dormant;
	ResolvedWeakPoints.Reset();
	WeakPointComboWindows.Reset();
	ResolvedWeakPointCount = 0;
	ComboTriggerCount = 0;
	ExpiredComboWindowCount = 0;
	LastComboText = TEXT("NO COMBO");
	LastComboWindowText = TEXT("WINDOWS CLOSED");

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsBossWeakPointActor(Actor))
		{
			continue;
		}

		if (UDSTargetableComponent* Targetable = Actor->FindComponentByClass<UDSTargetableComponent>())
		{
			Targetable->ClearReelExposure();
			Targetable->ClearFlyMark();
			Targetable->ClearLillyBind();
		}
	}
}

bool ADSDeepDockBossArenaActor::IsBossWeakPointActor(const AActor* Actor) const
{
	return Actor && Actor->ActorHasTag(TEXT("WeakPoint"));
}

void ADSDeepDockBossArenaActor::UpdateBossPhaseFromState()
{
	if (BossIntegrity <= 0.01f || ResolvedWeakPointCount >= WeakPointTargetCount)
	{
		BossIntegrity = 0.0f;
		ThreatLevel = 0.18f;
		bArenaArmed = false;
		BossPhase = EDSDeepDockBossPhase::Defeated;
		LastComboText = TEXT("BOSS DOWN: EXTRACT EVIDENCE");
		return;
	}

	if (ResolvedWeakPointCount >= 2)
	{
		BossPhase = EDSDeepDockBossPhase::Enraged;
		ThreatLevel = 1.0f;
		return;
	}

	if (ResolvedWeakPointCount >= 1)
	{
		BossPhase = EDSDeepDockBossPhase::Exposed;
		return;
	}

	BossPhase = EDSDeepDockBossPhase::Dormant;
}
