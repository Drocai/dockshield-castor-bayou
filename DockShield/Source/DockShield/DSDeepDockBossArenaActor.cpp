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
	BossPhase = EDSDeepDockBossPhase::Dormant;
	ResolvedWeakPointCount = 0;
	ComboTriggerCount = 0;
	LastComboText = TEXT("NO COMBO");

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

		float Readiness = 0.0f;
		Readiness += Targetable->IsReelExposed() ? 0.34f : 0.0f;
		Readiness += Targetable->IsFlyMarked() ? 0.33f : 0.0f;
		Readiness += Targetable->IsLillyBound() ? 0.33f : 0.0f;
		BestReadiness = FMath::Max(BestReadiness, Readiness);
	}

	return FMath::Clamp(BestReadiness, 0.0f, 1.0f);
}

int32 ADSDeepDockBossArenaActor::EvaluateBossWeakPointCombos()
{
	UWorld* World = GetWorld();
	if (!World || !bArenaArmed)
	{
		return 0;
	}

	int32 AppliedComboCount = 0;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (IsBossWeakPointActor(Actor) && ApplyHookLineSinkerCombo(Actor))
		{
			++AppliedComboCount;
		}
	}

	return AppliedComboCount;
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

	ResolvedWeakPoints.Add(FObjectKey(WeakPointActor));
	ResolvedWeakPointCount = ResolvedWeakPoints.Num();
	++ComboTriggerCount;

	BossIntegrity = FMath::Clamp(BossIntegrity - WeakPointDamagePerCombo, 0.0f, 1.0f);
	LabCoreIntegrity = FMath::Clamp(LabCoreIntegrity - 0.08f, 0.0f, 1.0f);
	ThreatLevel = FMath::Clamp(ThreatLevel + 0.045f, 0.0f, 1.0f);
	LastComboText = FString::Printf(TEXT("HOOK, LINE & SINKER x%d"), ComboTriggerCount);
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
	ResolvedWeakPointCount = 0;
	ComboTriggerCount = 0;
	LastComboText = TEXT("NO COMBO");

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
