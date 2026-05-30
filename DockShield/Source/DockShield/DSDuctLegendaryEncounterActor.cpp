#include "DSDuctLegendaryEncounterActor.h"
#include "DSTargetableComponent.h"

ADSDuctLegendaryEncounterActor::ADSDuctLegendaryEncounterActor()
{
    Tags.AddUnique(TEXT("Duct"));
    Tags.AddUnique(TEXT("LegendaryDuct"));
    Tags.AddUnique(TEXT("DockShieldTarget"));

    if (TargetableComponent)
    {
        TargetableComponent->TargetType = EDSTargetType::LegendaryDuct;
        TargetableComponent->DisplayName = FText::FromString(TEXT("Duct - Legendary Rubber Duck"));
        TargetableComponent->InteractionRange = 2200.0f;
        TargetableComponent->bCanReelPull = true;
        TargetableComponent->bCanFlyMark = true;
        TargetableComponent->bCanLillyBind = true;
    }
}

void ADSDuctLegendaryEncounterActor::BeginPlay()
{
    Super::BeginPlay();

    if (TargetableComponent)
    {
        TargetableComponent->ConfigureFromOwnerTags();
    }
}

bool ADSDuctLegendaryEncounterActor::BeginSighting()
{
    if (EncounterState == EDSDuctEncounterState::Hidden || EncounterState == EDSDuctEncounterState::Escaped)
    {
        ++SightingCount;
        EncounterProgress = 0.05f;
        bNearCatchRecordedThisAttempt = false;
    }

    EncounterState = EDSDuctEncounterState::Sighted;
    BestNearCatchProgress = FMath::Max(BestNearCatchProgress, EncounterProgress);
    return true;
}

bool ADSDuctLegendaryEncounterActor::ApplyBait(FName BaitId)
{
    BeginSighting();
    ++BaitAttemptCount;

    const bool bPremiumBait = BaitId == FName(TEXT("DuctTapeBait")) || BaitId == FName(TEXT("BobberBouncer"));
    LastBaitId = BaitId.IsNone() ? FName(TEXT("BasicBait")) : BaitId;
    LastBaitTier = bPremiumBait ? 2 : 1;
    LastBaitCost = bPremiumBait ? 3 : 1;
    EncounterProgress = FMath::Max(EncounterProgress, bPremiumBait ? 0.24f : 0.14f);
    BestNearCatchProgress = FMath::Max(BestNearCatchProgress, EncounterProgress);
    EncounterState = EDSDuctEncounterState::Lured;
    return true;
}

bool ADSDuctLegendaryEncounterActor::EvaluateRareSpawnForRoll(float Roll, float WeatherHazardScale, float MissionProgress, int32 BaitTier)
{
    LastSpawnRoll = FMath::Clamp(Roll, 0.0f, 1.0f);
    LastSpawnChance = CalculateSpawnChance(WeatherHazardScale, MissionProgress, BaitTier);
    const bool bShouldSpawn = LastSpawnRoll <= LastSpawnChance;
    if (bShouldSpawn)
    {
        BeginSighting();
    }

    return bShouldSpawn;
}

bool ADSDuctLegendaryEncounterActor::LatchWithReel(float InitialTension)
{
    if (EncounterState == EDSDuctEncounterState::Hidden || EncounterState == EDSDuctEncounterState::Escaped)
    {
        BeginSighting();
    }

    if (EncounterState != EDSDuctEncounterState::Latched && EncounterState != EDSDuctEncounterState::NearCatch)
    {
        ++AttemptCount;
        bNearCatchRecordedThisAttempt = false;
    }

    const float TensionBoost = FMath::Clamp(InitialTension, 0.0f, 1.0f) * 0.08f;
    EncounterProgress = FMath::Max(EncounterProgress, 0.18f + TensionBoost);
    BestNearCatchProgress = FMath::Max(BestNearCatchProgress, EncounterProgress);
    EncounterState = EDSDuctEncounterState::Latched;
    return true;
}

bool ADSDuctLegendaryEncounterActor::AdvanceReelBattle(float DeltaSeconds, float LineTension)
{
    if (DeltaSeconds <= 0.0f)
    {
        return EncounterState != EDSDuctEncounterState::Escaped;
    }

    if (EncounterState == EDSDuctEncounterState::Hidden || EncounterState == EDSDuctEncounterState::Sighted || EncounterState == EDSDuctEncounterState::Lured)
    {
        LatchWithReel(LineTension);
    }

    if (EncounterState == EDSDuctEncounterState::Escaped)
    {
        return false;
    }

    const float SafeTension = FMath::Clamp(LineTension, 0.0f, 1.0f);
    EncounterProgress += DeltaSeconds * (BaseProgressRate + (SafeTension * TensionProgressRate));
    EncounterProgress = FMath::Clamp(EncounterProgress, 0.0f, 1.0f);
    BestNearCatchProgress = FMath::Max(BestNearCatchProgress, EncounterProgress);

    if (EncounterProgress >= NearCatchThreshold)
    {
        EncounterState = EDSDuctEncounterState::NearCatch;
        if (!bNearCatchRecordedThisAttempt)
        {
            ++NearCatchCount;
            bNearCatchRecordedThisAttempt = true;
        }
    }

    const bool bOverTensioned = SafeTension >= 0.94f && EncounterProgress >= 0.72f;
    if (EncounterProgress >= SlipAwayThreshold || bOverTensioned)
    {
        ResolveSlipAway();
        return false;
    }

    return true;
}

void ADSDuctLegendaryEncounterActor::ResolveSlipAway()
{
    if (EncounterState == EDSDuctEncounterState::Escaped)
    {
        return;
    }

    if (EncounterProgress >= NearCatchThreshold && !bNearCatchRecordedThisAttempt)
    {
        ++NearCatchCount;
        bNearCatchRecordedThisAttempt = true;
    }

    if (EncounterProgress >= 0.72f)
    {
        ++DuctTapeTraceCount;
    }

    EncounterProgress = FMath::Clamp(EncounterProgress, 0.72f, 0.97f);
    BestNearCatchProgress = FMath::Max(BestNearCatchProgress, EncounterProgress);
    EncounterState = EDSDuctEncounterState::Escaped;
}

void ADSDuctLegendaryEncounterActor::ResetDuctEncounter()
{
    EncounterState = EDSDuctEncounterState::Hidden;
    EncounterProgress = 0.0f;
    BestNearCatchProgress = 0.0f;
    SightingCount = 0;
    BaitAttemptCount = 0;
    LastBaitId = FName(TEXT("None"));
    LastBaitTier = 0;
    LastBaitCost = 0;
    LastSpawnChance = 0.0f;
    LastSpawnRoll = 1.0f;
    AttemptCount = 0;
    NearCatchCount = 0;
    DuctTapeTraceCount = 0;
    bNearCatchRecordedThisAttempt = false;
}

FString ADSDuctLegendaryEncounterActor::GetDuctStatusText() const
{
    return FString::Printf(
        TEXT("DUCT %s | BEST %.0f%% | SIGHT %d | BAIT %d | NEAR %d | TAPE %d | UNCAUGHT"),
        *GetEncounterStateText(),
        BestNearCatchProgress * 100.0f,
        SightingCount,
        BaitAttemptCount,
        NearCatchCount,
        DuctTapeTraceCount);
}

FString ADSDuctLegendaryEncounterActor::GetRarityStatusText() const
{
    return FString::Printf(
        TEXT("DUCT RARITY %.1f%% | ROLL %.1f%% | TIER %d | %s"),
        LastSpawnChance * 100.0f,
        LastSpawnRoll * 100.0f,
        LastBaitTier,
        EncounterState == EDSDuctEncounterState::Hidden ? TEXT("HIDDEN") : TEXT("ACTIVE"));
}

FString ADSDuctLegendaryEncounterActor::GetBaitEconomyStatusText() const
{
    return FString::Printf(
        TEXT("DUCT BAIT %s | COST %d SAMPLE | ATTEMPTS %d | CANNOT CATCH"),
        *LastBaitId.ToString(),
        LastBaitCost,
        BaitAttemptCount);
}

float ADSDuctLegendaryEncounterActor::CalculateSpawnChance(float WeatherHazardScale, float MissionProgress, int32 BaitTier) const
{
    const float HazardBonus = FMath::Max(0.0f, WeatherHazardScale - 1.0f) * WeatherHazardSpawnBonus;
    const float ProgressBonus = FMath::Clamp(MissionProgress, 0.0f, 1.0f) * MissionProgressSpawnBonus;
    const float BaitBonus = FMath::Clamp(static_cast<float>(BaitTier), 0.0f, 3.0f) * BaitTierSpawnBonus;
    return FMath::Clamp(BaseRareSpawnChance + HazardBonus + ProgressBonus + BaitBonus, 0.0f, MaxRareSpawnChance);
}

FString ADSDuctLegendaryEncounterActor::GetEncounterStateText() const
{
    switch (EncounterState)
    {
    case EDSDuctEncounterState::Sighted:
        return TEXT("SIGHTED");
    case EDSDuctEncounterState::Lured:
        return TEXT("LURED");
    case EDSDuctEncounterState::Latched:
        return TEXT("LATCHED");
    case EDSDuctEncounterState::NearCatch:
        return TEXT("ALMOST");
    case EDSDuctEncounterState::Escaped:
        return TEXT("SLIPPED");
    default:
        return TEXT("HIDDEN");
    }
}
