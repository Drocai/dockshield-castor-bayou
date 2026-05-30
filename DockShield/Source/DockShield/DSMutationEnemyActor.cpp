#include "DSMutationEnemyActor.h"
#include "DSTargetableComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

ADSMutationEnemyActor::ADSMutationEnemyActor()
{
    PrimaryActorTick.bCanEverTick = true;

    MutationName = TEXT("BAYOU MUTATION BRUISER");
    ComboDamage = 0.34f;
    LastCombatText = TEXT("WAITING FOR CONTACT");
    LastFeedbackText = TEXT("COMBAT FEEDBACK READY");

    FeedbackLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MutationFeedbackLight"));
    FeedbackLight->SetupAttachment(GetStaticMeshComponent());
    FeedbackLight->SetRelativeLocation(FVector(0.0f, 0.0f, 128.0f));
    FeedbackLight->SetIntensity(0.0f);
    FeedbackLight->SetAttenuationRadius(520.0f);
    FeedbackLight->SetLightColor(FLinearColor(0.27f, 1.0f, 0.55f, 1.0f));

    FeedbackRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MutationFeedbackRing"));
    FeedbackRingMesh->SetupAttachment(GetStaticMeshComponent());
    FeedbackRingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -54.0f));
    FeedbackRingMesh->SetRelativeScale3D(FVector(1.1f, 1.1f, 0.018f));
    FeedbackRingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FeedbackRingMesh->SetVisibility(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (CylinderMesh.Succeeded())
    {
        FeedbackRingMesh->SetStaticMesh(CylinderMesh.Object);
    }

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

void ADSMutationEnemyActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (FeedbackPulseRemaining <= 0.0f)
    {
        if (FeedbackLight)
        {
            FeedbackLight->SetIntensity(0.0f);
        }
        if (FeedbackRingMesh)
        {
            FeedbackRingMesh->SetVisibility(false);
        }
        return;
    }

    FeedbackPulseRemaining = FMath::Max(0.0f, FeedbackPulseRemaining - DeltaSeconds);
    const float Duration = FMath::Max(0.01f, FeedbackPulseDuration);
    const float Alpha = FMath::Clamp(FeedbackPulseRemaining / Duration, 0.0f, 1.0f);
    const float Pulse = 0.35f + (0.65f * Alpha);
    const FColor PulseColor = LastFeedbackColor.ToFColor(true);

    if (FeedbackLight)
    {
        FeedbackLight->SetLightColor(LastFeedbackColor);
        FeedbackLight->SetIntensity(2800.0f * FeedbackPulseIntensity * Pulse);
    }

    if (FeedbackRingMesh)
    {
        FeedbackRingMesh->SetVisibility(true);
        const float RingScale = 1.1f + ((1.0f - Alpha) * 1.6f);
        FeedbackRingMesh->SetRelativeScale3D(FVector(RingScale, RingScale, 0.018f));
    }

    if (UWorld* World = GetWorld())
    {
        DrawDebugSphere(World, GetActorLocation() + FVector(0.0f, 0.0f, 96.0f), 86.0f + ((1.0f - Alpha) * 96.0f), 16, PulseColor, false, 0.0f, 0, 2.0f);
        DrawDebugString(World, GetActorLocation() + FVector(0.0f, 0.0f, 190.0f), LastFeedbackText, nullptr, PulseColor, 0.0f, true);
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
    TriggerCombatFeedback(FName(TEXT("ReelStagger")), TEXT("REEL STAGGER: shock line hit"), FLinearColor(1.0f, 0.58f, 0.10f, 1.0f), 0.85f, 0.70f);
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
    TriggerCombatFeedback(FName(TEXT("FlyMark")), TEXT("FLY MARK: sonar outline locked"), FLinearColor(0.0f, 0.92f, 0.72f, 1.0f), 0.72f, 0.62f);
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
    TriggerCombatFeedback(FName(TEXT("LillyBind")), TEXT("LILLY BIND: root snare locked"), FLinearColor(0.54f, 1.0f, 0.16f, 1.0f), 0.78f, 0.74f);
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
    TriggerCombatFeedback(FName(TEXT("HookLineSinkerImpact")), FString::Printf(TEXT("HOOK, LINE & SINKER IMPACT x%d"), ComboImpactCount), FLinearColor(1.0f, 0.18f, 0.10f, 1.0f), 1.25f, 1.10f);

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
    LastFeedbackEvent = FName(TEXT("Reset"));
    LastFeedbackText = TEXT("COMBAT FEEDBACK RESET");
    FeedbackPulseRemaining = 0.0f;
    FeedbackPulseDuration = 0.0f;
    FeedbackPulseIntensity = 0.0f;
    if (FeedbackLight)
    {
        FeedbackLight->SetIntensity(0.0f);
    }
    if (FeedbackRingMesh)
    {
        FeedbackRingMesh->SetVisibility(false);
    }

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
        TriggerCombatFeedback(FName(TEXT("MutationDefeated")), TEXT("MUTATION DEFEATED: extract evidence"), FLinearColor(1.0f, 0.08f, 0.04f, 1.0f), 1.35f, 1.25f);
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

void ADSMutationEnemyActor::TriggerCombatFeedback(FName EventName, const FString& FeedbackText, const FLinearColor& FeedbackColor, float Intensity, float Duration)
{
    LastFeedbackEvent = EventName;
    LastFeedbackText = FeedbackText;
    LastFeedbackColor = FeedbackColor;
    FeedbackPulseIntensity = FMath::Clamp(Intensity, 0.0f, 2.0f);
    FeedbackPulseDuration = FMath::Max(0.05f, Duration);
    FeedbackPulseRemaining = FeedbackPulseDuration;
    ++FeedbackPulseCount;
}
