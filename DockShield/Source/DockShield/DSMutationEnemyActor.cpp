#include "DSMutationEnemyActor.h"
#include "DSTargetableComponent.h"

ADSMutationEnemyActor::ADSMutationEnemyActor()
{
    PrimaryActorTick.bCanEverTick = false;

    MutationName = TEXT("BAYOU MUTATION BRUISER");
    ComboDamage = 0.34f;
    LastCombatText = TEXT("WAITING FOR CONTACT");

    Tags.AddUnique(TEXT("DockShieldTarget"));
    Tags.AddUnique(TEXT("Enemy"));
    Tags.AddUnique(TEXT("MutationEnemy"));

    if (TargetableComponent)
    {
        TargetableComponent->TargetType = EDSTargetType::Enemy;
        TargetableComponent->DisplayName = FText::FromString(TEXT("Bayou Mutation Bruiser"));
        TargetableComponent->InteractionRange = 1850.0f;
        TargetableComponent->bCanReelPull = true;
        TargetableComponent->bCanFlyMark = true;
        TargetableComponent->bCanLillyBind = true;
    }
}

void ADSMutationEnemyActor::BeginPlay()
{
    Super::BeginPlay();

    if (TargetableComponent)
    {
        TargetableComponent->ConfigureFromOwnerTags();
    }
}

bool ADSMutationEnemyActor::ApplyReelImpact(float Strength)
{
    if (IsDefeated() || !TargetableComponent)
    {
        return false;
    }

    TargetableComponent->ConfigureFromOwnerTags();
    if (!TargetableComponent->ExposeForReel(Strength))
    {
        return false;
    }

    const float SafeStrength = FMath::Clamp(Strength, 0.0f, 1.0f);
    ++ReelImpactCount;
    ThreatState = EDSMutationThreatState::Staggered;
    Stagger = FMath::Clamp(Stagger + 0.34f + (SafeStrength * 0.24f), 0.0f, 1.0f);
    Aggression = FMath::Clamp(Aggression + 0.08f, 0.0f, 1.0f);
    ApplyDamage(0.10f + (SafeStrength * 0.06f), TEXT("REEL STAGGER"));
    EvaluateHeroCombo();
    return true;
}

bool ADSMutationEnemyActor::ApplyFlyPressure(float Strength)
{
    if (IsDefeated() || !TargetableComponent)
    {
        return false;
    }

    TargetableComponent->ConfigureFromOwnerTags();
    if (!TargetableComponent->MarkForFly(Strength))
    {
        return false;
    }

    const float SafeStrength = FMath::Clamp(Strength, 0.0f, 1.0f);
    ++FlyPressureCount;
    if (ThreatState == EDSMutationThreatState::Dormant)
    {
        ThreatState = EDSMutationThreatState::Alert;
    }

    Vulnerability = FMath::Clamp(Vulnerability + 0.26f + (SafeStrength * 0.18f), 0.0f, 1.0f);
    Aggression = FMath::Clamp(Aggression + 0.04f, 0.0f, 1.0f);
    ApplyDamage(0.04f + (SafeStrength * 0.04f), TEXT("FLY MARK"));
    EvaluateHeroCombo();
    return true;
}

bool ADSMutationEnemyActor::ApplyLillyPressure(float Strength)
{
    if (IsDefeated() || !TargetableComponent)
    {
        return false;
    }

    TargetableComponent->ConfigureFromOwnerTags();
    if (!TargetableComponent->BindForLilly(Strength))
    {
        return false;
    }

    const float SafeStrength = FMath::Clamp(Strength, 0.0f, 1.0f);
    ++LillyPressureCount;
    ThreatState = EDSMutationThreatState::Bound;
    Stagger = FMath::Clamp(Stagger + 0.20f + (SafeStrength * 0.18f), 0.0f, 1.0f);
    Aggression = FMath::Clamp(Aggression - 0.03f, 0.0f, 1.0f);
    ApplyDamage(0.07f + (SafeStrength * 0.05f), TEXT("LILLY BIND"));
    EvaluateHeroCombo();
    return true;
}

bool ADSMutationEnemyActor::EvaluateHeroCombo()
{
    if (IsDefeated() || !TargetableComponent)
    {
        return false;
    }

    TargetableComponent->ConfigureFromOwnerTags();
    if (!TargetableComponent->IsHookLineSinkerReady())
    {
        return false;
    }

    const int32 ComboSignature = GetCurrentComboSignature();
    if (ComboSignature == 0 || ComboSignature == LastResolvedComboSignature)
    {
        return false;
    }

    LastResolvedComboSignature = ComboSignature;
    ++ComboImpactCount;

    const float VulnerabilityBonus = FMath::Clamp(Vulnerability * 0.10f, 0.0f, 0.10f);
    ApplyDamage(ComboDamage + VulnerabilityBonus, FString::Printf(TEXT("HOOK, LINE & SINKER x%d"), ComboImpactCount));
    Aggression = FMath::Clamp(Aggression + 0.12f, 0.0f, 1.0f);
    Stagger = 1.0f;

    if (!IsDefeated())
    {
        ThreatState = EDSMutationThreatState::Staggered;
        TargetableComponent->ClearReelExposure();
        TargetableComponent->ClearFlyMark();
        TargetableComponent->ClearLillyBind();
    }

    return true;
}

void ADSMutationEnemyActor::ResetMutationEncounter()
{
    ThreatState = EDSMutationThreatState::Dormant;
    Health = 1.0f;
    Aggression = 0.35f;
    Stagger = 0.0f;
    Vulnerability = 0.0f;
    ReelImpactCount = 0;
    FlyPressureCount = 0;
    LillyPressureCount = 0;
    ComboImpactCount = 0;
    LastResolvedComboSignature = 0;
    LastCombatText = TEXT("RESET");

    if (TargetableComponent)
    {
        TargetableComponent->ClearReelExposure();
        TargetableComponent->ClearFlyMark();
        TargetableComponent->ClearLillyBind();
        TargetableComponent->ConfigureFromOwnerTags();
    }
}

FString ADSMutationEnemyActor::GetMutationStatusText() const
{
    return FString::Printf(
        TEXT("%s | %s | HEALTH %d%% | AGGRO %d%% | COMBO %d | %s"),
        *MutationName,
        *GetThreatStateText(),
        FMath::RoundToInt(Health * 100.0f),
        FMath::RoundToInt(Aggression * 100.0f),
        ComboImpactCount,
        *LastCombatText);
}

FString ADSMutationEnemyActor::GetThreatStateText() const
{
    switch (ThreatState)
    {
    case EDSMutationThreatState::Alert:
        return TEXT("ALERT");
    case EDSMutationThreatState::Staggered:
        return TEXT("STAGGERED");
    case EDSMutationThreatState::Bound:
        return TEXT("BOUND");
    case EDSMutationThreatState::Defeated:
        return TEXT("DEFEATED");
    default:
        return TEXT("DORMANT");
    }
}

void ADSMutationEnemyActor::ApplyDamage(float DamageAmount, const FString& SourceText)
{
    if (IsDefeated())
    {
        return;
    }

    Health = FMath::Clamp(Health - FMath::Max(0.0f, DamageAmount), 0.0f, 1.0f);
    LastCombatText = SourceText;

    if (Health <= 0.01f)
    {
        Health = 0.0f;
        Aggression = 0.0f;
        Stagger = 1.0f;
        ThreatState = EDSMutationThreatState::Defeated;
        LastCombatText = FString::Printf(TEXT("%s: DEFEATED"), *SourceText);
        if (TargetableComponent)
        {
            TargetableComponent->ClearReelExposure();
            TargetableComponent->ClearFlyMark();
            TargetableComponent->ClearLillyBind();
        }
    }
}

int32 ADSMutationEnemyActor::GetCurrentComboSignature() const
{
    if (!TargetableComponent)
    {
        return 0;
    }

    return (TargetableComponent->ReelExposureCount * 10000)
        + (TargetableComponent->FlyMarkCount * 100)
        + TargetableComponent->LillyBindCount;
}
