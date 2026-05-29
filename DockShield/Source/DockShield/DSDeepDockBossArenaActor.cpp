#include "DSDeepDockBossArenaActor.h"

ADSDeepDockBossArenaActor::ADSDeepDockBossArenaActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ArenaName = TEXT("DEEP DOCK BOSS ARENA");
	BossName = TEXT("DEEP DOCK MUTATION");
	BossIntegrity = 1.0f;
	LabCoreIntegrity = 0.72f;
	ThreatLevel = 0.85f;
	bArenaArmed = true;

	Tags.AddUnique(FName(TEXT("DockShieldBossArena")));
}

FString ADSDeepDockBossArenaActor::GetArenaStatusText() const
{
	const int32 BossPercent = FMath::RoundToInt(BossIntegrity * 100.0f);
	const int32 CorePercent = FMath::RoundToInt(LabCoreIntegrity * 100.0f);
	const int32 ThreatPercent = FMath::RoundToInt(ThreatLevel * 100.0f);

	return FString::Printf(
		TEXT("%s | %s %d%% | CORE %d%% | THREAT %d%%"),
		*ArenaName,
		*BossName,
		BossPercent,
		CorePercent,
		ThreatPercent);
}
