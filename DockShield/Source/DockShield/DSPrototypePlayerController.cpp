#include "DSPrototypePlayerController.h"
#include "DSDeepDockBossArenaActor.h"
#include "DSFlyPrototypeCharacter.h"
#include "DSLillyPrototypeCharacter.h"
#include "DSMutationEnemyActor.h"
#include "DSReelPrototypeCharacter.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "InputCoreTypes.h"

ADSPrototypePlayerController::ADSPrototypePlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    ReelClass = ADSReelPrototypeCharacter::StaticClass();
    FlyClass = ADSFlyPrototypeCharacter::StaticClass();
    LillyClass = ADSLillyPrototypeCharacter::StaticClass();
}

void ADSPrototypePlayerController::BeginPlay()
{
    Super::BeginPlay();
    RegisterCurrentPawn();
    RefreshBossArenaAwareness();
    RefreshMutationEncounterAwareness();
    ShowSwitchMessage();
}

void ADSPrototypePlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    BossArenaScanAccumulator += DeltaSeconds;
    if (BossArenaScanAccumulator >= 0.5f)
    {
        BossArenaScanAccumulator = 0.0f;
        RefreshBossArenaAwareness();
    }

    MutationScanAccumulator += DeltaSeconds;
    if (MutationScanAccumulator >= 0.5f)
    {
        MutationScanAccumulator = 0.0f;
        RefreshMutationEncounterAwareness();
    }

    CombatFeedbackFlash = FMath::Max(0.0f, CombatFeedbackFlash - DeltaSeconds);
}

void ADSPrototypePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent)
    {
        return;
    }

    InputComponent->BindKey(EKeys::One, IE_Pressed, this, &ADSPrototypePlayerController::HandleSwitchToReelInput);
    InputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ADSPrototypePlayerController::HandleSwitchToFlyInput);
    InputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ADSPrototypePlayerController::HandleSwitchToLillyInput);
    InputComponent->BindKey(EKeys::Tab, IE_Pressed, this, &ADSPrototypePlayerController::HandleCycleHeroInput);
    InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &ADSPrototypePlayerController::HandleToggleSettingsInput);
    InputComponent->BindKey(EKeys::P, IE_Pressed, this, &ADSPrototypePlayerController::HandleToggleSettingsInput);
    InputComponent->BindKey(EKeys::K, IE_Pressed, this, &ADSPrototypePlayerController::HandleCycleWeatherInput);
    InputComponent->BindKey(EKeys::O, IE_Pressed, this, &ADSPrototypePlayerController::CycleVisualQuality);
    InputComponent->BindKey(EKeys::LeftBracket, IE_Pressed, this, &ADSPrototypePlayerController::DecreaseHudScale);
    InputComponent->BindKey(EKeys::RightBracket, IE_Pressed, this, &ADSPrototypePlayerController::IncreaseHudScale);
    InputComponent->BindKey(EKeys::Hyphen, IE_Pressed, this, &ADSPrototypePlayerController::DecreaseGamma);
    InputComponent->BindKey(EKeys::Equals, IE_Pressed, this, &ADSPrototypePlayerController::IncreaseGamma);
}

bool ADSPrototypePlayerController::SwitchToReel()
{
    return SwitchToRole(EDSPrototypeHeroRole::Reel);
}

bool ADSPrototypePlayerController::SwitchToFly()
{
    return SwitchToRole(EDSPrototypeHeroRole::Fly);
}

bool ADSPrototypePlayerController::SwitchToLilly()
{
    return SwitchToRole(EDSPrototypeHeroRole::Lilly);
}

bool ADSPrototypePlayerController::CycleHero()
{
    switch (ActiveRole)
    {
    case EDSPrototypeHeroRole::Reel:
        return SwitchToRole(EDSPrototypeHeroRole::Fly);
    case EDSPrototypeHeroRole::Fly:
        return SwitchToRole(EDSPrototypeHeroRole::Lilly);
    default:
        return SwitchToRole(EDSPrototypeHeroRole::Reel);
    }
}

void ADSPrototypePlayerController::HandleSwitchToReelInput()
{
    SwitchToReel();
}

void ADSPrototypePlayerController::HandleSwitchToFlyInput()
{
    SwitchToFly();
}

void ADSPrototypePlayerController::HandleSwitchToLillyInput()
{
    SwitchToLilly();
}

void ADSPrototypePlayerController::HandleCycleHeroInput()
{
    CycleHero();
}

void ADSPrototypePlayerController::HandleToggleSettingsInput()
{
    ToggleSettingsPanel();
}

void ADSPrototypePlayerController::HandleCycleWeatherInput()
{
    CycleWeatherState();
}

FString ADSPrototypePlayerController::GetActiveHeroLabel() const
{
    switch (ActiveRole)
    {
    case EDSPrototypeHeroRole::Reel:
        return TEXT("THE REEL");
    case EDSPrototypeHeroRole::Fly:
        return TEXT("THE FLY");
    default:
        return TEXT("LILLY LOCH");
    }
}

FString ADSPrototypePlayerController::GetSwitchStatusText() const
{
    return FString::Printf(TEXT("%s | 1 REEL  2 FLY  3 LILLY  TAB"), *LastSwitchResult);
}

int32 ADSPrototypePlayerController::GetSwitchCount() const
{
    return SwitchCount;
}

AActor* ADSPrototypePlayerController::GetReelPawnActor() const
{
    return ReelPawn.Get();
}

AActor* ADSPrototypePlayerController::GetFlyPawnActor() const
{
    return FlyPawn.Get();
}

AActor* ADSPrototypePlayerController::GetLillyPawnActor() const
{
    return LillyPawn.Get();
}

void ADSPrototypePlayerController::NotifyPrototypeAction(FName ActionName, int32 CreditReward, int32 XPReward, int32 SampleReward)
{
    const int32 SafeCredits = FMath::Max(0, CreditReward);
    const int32 SafeXP = FMath::Max(0, XPReward);
    const int32 SafeSamples = FMath::Max(0, SampleReward);

    BetaCredits += SafeCredits;
    PrototypeXP += SafeXP;
    DepthSamples += SafeSamples;

    LastEconomyEvent = FString::Printf(
        TEXT("%s +%d CR +%d XP +%d SAMPLE"),
        *ActionName.ToString(),
        SafeCredits,
        SafeXP,
        SafeSamples);

    AdvanceMissionFromAction(ActionName);
    RoutePrototypeCuesForAction(ActionName);

    if (ActionName == FName(TEXT("ReelCast")))
    {
        SetCombatFeedback(ActionName, TEXT("REEL CAST: line out"));
        UnlockAchievement(FName(TEXT("FIRST_CAST")));
    }
    else if (ActionName == FName(TEXT("CivilianRescue")))
    {
        SetCombatFeedback(ActionName, TEXT("RESCUE REEL: civilian extracted"), 1.25f);
        UnlockAchievement(FName(TEXT("FIRST_RESCUE")));
    }
    else if (ActionName == FName(TEXT("BoatTow")))
    {
        SetCombatFeedback(ActionName, TEXT("BOAT TOW: hull line attached"));
        UnlockAchievement(FName(TEXT("FIRST_BOAT_TOW")));
    }
    else if (ActionName == FName(TEXT("GrapplePull")))
    {
        SetCombatFeedback(ActionName, TEXT("GRAPPLE PULL: anchor momentum"));
        UnlockAchievement(FName(TEXT("FIRST_GRAPPLE_PULL")));
    }
    else if (ActionName == FName(TEXT("FlyMark")))
    {
        SetCombatFeedback(ActionName, TEXT("FLY MARK: sonar outline locked"));
        UnlockAchievement(FName(TEXT("FIRST_FLY_MARK")));
    }
    else if (ActionName == FName(TEXT("LillyBind")))
    {
        SetCombatFeedback(ActionName, TEXT("LILLY BIND: root snare locked"));
        UnlockAchievement(FName(TEXT("FIRST_LILLY_BIND")));
    }
    else if (ActionName == FName(TEXT("BossWeakPointExposed")))
    {
        SetCombatFeedback(ActionName, TEXT("WEAK POINT EXPOSED: Reel window open"), 1.2f);
        UnlockAchievement(FName(TEXT("FIRST_BOSS_EXPOSE")));
    }
    else if (ActionName == FName(TEXT("HookLineSinker")))
    {
        SetCombatFeedback(ActionName, TEXT("HOOK, LINE & SINKER: combo impact"), 1.35f);
        UnlockAchievement(FName(TEXT("FIRST_HOOK_LINE_SINKER")));
    }
    else if (ActionName == FName(TEXT("BossDefeated")))
    {
        SetCombatFeedback(ActionName, TEXT("DEEP DOCK THREAT DOWN: extract evidence"), 1.5f);
        UnlockAchievement(FName(TEXT("BOSS_DEFEATED")));
    }
    else if (ActionName == FName(TEXT("MutationStagger")))
    {
        SetCombatFeedback(ActionName, TEXT("REEL STAGGER: mutation shock line"));
        UnlockAchievement(FName(TEXT("FIRST_MUTATION_STAGGER")));
    }
    else if (ActionName == FName(TEXT("MutationCombo")))
    {
        SetCombatFeedback(ActionName, TEXT("MUTATION COMBO: Hook, Line & Sinker hit"), 1.35f);
        UnlockAchievement(FName(TEXT("FIRST_MUTATION_COMBO")));
    }
    else if (ActionName == FName(TEXT("MutationDefeated")))
    {
        SetCombatFeedback(ActionName, TEXT("MUTATION DEFEATED: samples secured"), 1.5f);
        UnlockAchievement(FName(TEXT("FIRST_MUTATION_DEFEATED")));
    }
    else if (ActionName == FName(TEXT("DuctSighting")))
    {
        ++DuctSightings;
        SetCombatFeedback(ActionName, TEXT("DUCT SIGHTING: golden wake ping"), 1.2f);
        UnlockAchievement(FName(TEXT("DUCT_FIRST_SIGHTING")));
    }
    else if (ActionName == FName(TEXT("DuctNearCatch")))
    {
        ++DuctNearCatchCount;
        ++DuctTapeFragments;
        SetCombatFeedback(ActionName, TEXT("DUCT NEAR-CATCH: tension spike, still uncatchable"), 1.35f);
        UnlockAchievement(FName(TEXT("DUCT_ALMOST_HAD_HIM")));
        if (DuctTapeFragments >= 3)
        {
            UnlockAchievement(FName(TEXT("DUCT_TAPE_EVIDENCE")));
        }
    }
    else if (ActionName == FName(TEXT("DuctTrace")))
    {
        ++DuctTapeFragments;
        SetCombatFeedback(ActionName, TEXT("DUCT TRACE: tape fragment logged"));
        UnlockAchievement(FName(TEXT("DUCT_TAPE_EVIDENCE")));
    }
    else if (ActionName == FName(TEXT("LineSnap")))
    {
        SetCombatFeedback(ActionName, TEXT("LINE SNAP: extraction pressure rising"), 1.15f);
    }
}

bool ADSPrototypePlayerController::UnlockAchievement(FName AchievementId)
{
    if (AchievementId.IsNone() || UnlockedAchievements.Contains(AchievementId))
    {
        return false;
    }

    UnlockedAchievements.Add(AchievementId);
    PrototypeXP += 15;
    LastAchievementText = GetAchievementLabel(AchievementId);
    return true;
}

FString ADSPrototypePlayerController::GetEconomyStatusText() const
{
    return FString::Printf(
        TEXT("CREDITS %d | XP %d | SAMPLES %d | %s"),
        BetaCredits,
        PrototypeXP,
        DepthSamples,
        *LastEconomyEvent);
}

FString ADSPrototypePlayerController::GetAchievementStatusText() const
{
    return FString::Printf(
        TEXT("ACHIEVEMENTS %d/17 | LAST: %s"),
        UnlockedAchievements.Num(),
        *LastAchievementText);
}

FString ADSPrototypePlayerController::GetBossArenaStatusText() const
{
    return LastBossArenaStatus;
}

FString ADSPrototypePlayerController::GetMutationStatusText() const
{
    return LastMutationStatus;
}

FString ADSPrototypePlayerController::GetDuctStatusText() const
{
    return FString::Printf(
        TEXT("DUCT: UNCAUGHT | SIGHT %d | NEAR %d | TAPE %d"),
        DuctSightings,
        DuctNearCatchCount,
        DuctTapeFragments);
}

FString ADSPrototypePlayerController::GetCombatFeedbackStatusText() const
{
    return FString::Printf(
        TEXT("%s | EVENTS %d"),
        *LastCombatFeedbackText,
        CombatFeedbackEventCount);
}

void ADSPrototypePlayerController::CycleWeatherState()
{
    if (WeatherState == EDSPrototypeWeatherState::HeavyRain)
    {
        WeatherState = EDSPrototypeWeatherState::FogBank;
    }
    else if (WeatherState == EDSPrototypeWeatherState::FogBank)
    {
        WeatherState = EDSPrototypeWeatherState::ToxicRain;
    }
    else if (WeatherState == EDSPrototypeWeatherState::ToxicRain)
    {
        WeatherState = EDSPrototypeWeatherState::FloodSurge;
    }
    else
    {
        WeatherState = EDSPrototypeWeatherState::HeavyRain;
    }

    ++WeatherCycleCount;
    MissionObjectiveText = FString::Printf(TEXT("OBJECTIVE: Adapt to %s"), *GetWeatherStateLabel());
    SetVisualCue(FName(TEXT("VFX_WEATHER_SHIFT")), GetWeatherHazardPressureScale());

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(82035, 1.75f, FColor::Cyan, GetWeatherStatusText());
    }
}

FString ADSPrototypePlayerController::GetWeatherStateLabel() const
{
    switch (WeatherState)
    {
    case EDSPrototypeWeatherState::FogBank:
        return TEXT("FOG BANK");
    case EDSPrototypeWeatherState::ToxicRain:
        return TEXT("TOXIC RAIN");
    case EDSPrototypeWeatherState::FloodSurge:
        return TEXT("FLOOD SURGE");
    default:
        return TEXT("HEAVY RAIN");
    }
}

FString ADSPrototypePlayerController::GetWeatherStatusText() const
{
    return FString::Printf(
        TEXT("WEATHER %s | VIS %.0f%% | RANGE %.0f%% | HAZARD %.0f%% | K CYCLE"),
        *GetWeatherStateLabel(),
        GetWeatherVisibilityScale() * 100.0f,
        GetWeatherTargetRangeScale() * 100.0f,
        GetWeatherHazardPressureScale() * 100.0f);
}

float ADSPrototypePlayerController::GetWeatherTargetRangeScale() const
{
    switch (WeatherState)
    {
    case EDSPrototypeWeatherState::FogBank:
        return 0.72f;
    case EDSPrototypeWeatherState::ToxicRain:
        return 0.82f;
    case EDSPrototypeWeatherState::FloodSurge:
        return 0.96f;
    default:
        return 0.9f;
    }
}

float ADSPrototypePlayerController::GetWeatherVisibilityScale() const
{
    switch (WeatherState)
    {
    case EDSPrototypeWeatherState::FogBank:
        return 0.48f;
    case EDSPrototypeWeatherState::ToxicRain:
        return 0.62f;
    case EDSPrototypeWeatherState::FloodSurge:
        return 0.76f;
    default:
        return 0.7f;
    }
}

float ADSPrototypePlayerController::GetWeatherHazardPressureScale() const
{
    switch (WeatherState)
    {
    case EDSPrototypeWeatherState::FogBank:
        return 1.04f;
    case EDSPrototypeWeatherState::ToxicRain:
        return 1.45f;
    case EDSPrototypeWeatherState::FloodSurge:
        return 1.65f;
    default:
        return 1.12f;
    }
}

FString ADSPrototypePlayerController::GetMissionObjectiveStatusText() const
{
    return FString::Printf(
        TEXT("%s | PROGRESS %.0f%%"),
        *MissionObjectiveText,
        FMath::Clamp(MissionObjectiveProgress, 0.0f, 1.0f) * 100.0f);
}

FString ADSPrototypePlayerController::GetExtractionStatusText() const
{
    return FString::Printf(
        TEXT("EXTRACTION SECURED %d | FAILED %d | WEATHER CYCLES %d"),
        ExtractionSecuredCount,
        ExtractionFailureCount,
        WeatherCycleCount);
}

FString ADSPrototypePlayerController::GetAudioCueStatusText() const
{
    return FString::Printf(
        TEXT("SFX %s | INT %.0f%% | EVENTS %d"),
        *LastAudioCueId.ToString(),
        LastAudioCueIntensity * 100.0f,
        AudioCueEventCount);
}

FString ADSPrototypePlayerController::GetVisualCueStatusText() const
{
    return FString::Printf(
        TEXT("VFX %s | INT %.0f%% | EVENTS %d"),
        *LastVisualCueId.ToString(),
        LastVisualCueIntensity * 100.0f,
        VisualCueEventCount);
}

FString ADSPrototypePlayerController::GetSettingsStatusText() const
{
    return FString::Printf(
        TEXT("SETTINGS %s | HUD %.0f%% | GAMMA %.2f | VISUAL %s"),
        bSettingsPanelOpen ? TEXT("OPEN") : TEXT("P/ESC"),
        HudScale * 100.0f,
        Gamma,
        *GetVisualQualityLabel());
}

FString ADSPrototypePlayerController::GetVisualQualityLabel() const
{
    switch (VisualQuality)
    {
    case EDSPrototypeVisualQuality::Performance:
        return TEXT("PERFORMANCE");
    case EDSPrototypeVisualQuality::Cinematic:
        return TEXT("CINEMATIC");
    default:
        return TEXT("BALANCED");
    }
}

void ADSPrototypePlayerController::ToggleSettingsPanel()
{
    bSettingsPanelOpen = !bSettingsPanelOpen;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(82033, 1.5f, FColor::Silver, GetSettingsStatusText());
    }
}

void ADSPrototypePlayerController::CycleVisualQuality()
{
    if (VisualQuality == EDSPrototypeVisualQuality::Performance)
    {
        VisualQuality = EDSPrototypeVisualQuality::Balanced;
    }
    else if (VisualQuality == EDSPrototypeVisualQuality::Balanced)
    {
        VisualQuality = EDSPrototypeVisualQuality::Cinematic;
    }
    else
    {
        VisualQuality = EDSPrototypeVisualQuality::Performance;
    }
}

void ADSPrototypePlayerController::IncreaseHudScale()
{
    HudScale = FMath::Clamp(HudScale + 0.05f, 0.75f, 1.35f);
}

void ADSPrototypePlayerController::DecreaseHudScale()
{
    HudScale = FMath::Clamp(HudScale - 0.05f, 0.75f, 1.35f);
}

void ADSPrototypePlayerController::IncreaseGamma()
{
    Gamma = FMath::Clamp(Gamma + 0.05f, 0.75f, 1.35f);
}

void ADSPrototypePlayerController::DecreaseGamma()
{
    Gamma = FMath::Clamp(Gamma - 0.05f, 0.75f, 1.35f);
}

void ADSPrototypePlayerController::RegisterCurrentPawn()
{
    if (ADSReelPrototypeCharacter* ControlledReel = Cast<ADSReelPrototypeCharacter>(GetPawn()))
    {
        ReelPawn = ControlledReel;
        ActiveRole = EDSPrototypeHeroRole::Reel;
        LastSwitchResult = TEXT("ACTIVE: THE REEL");
    }
    else if (ADSFlyPrototypeCharacter* ControlledFly = Cast<ADSFlyPrototypeCharacter>(GetPawn()))
    {
        FlyPawn = ControlledFly;
        ActiveRole = EDSPrototypeHeroRole::Fly;
        LastSwitchResult = TEXT("ACTIVE: THE FLY");
    }
    else if (ADSLillyPrototypeCharacter* ControlledLilly = Cast<ADSLillyPrototypeCharacter>(GetPawn()))
    {
        LillyPawn = ControlledLilly;
        ActiveRole = EDSPrototypeHeroRole::Lilly;
        LastSwitchResult = TEXT("ACTIVE: LILLY LOCH");
    }
}

bool ADSPrototypePlayerController::SwitchToRole(EDSPrototypeHeroRole TargetRole)
{
    RegisterCurrentPawn();

    UWorld* World = GetWorld();
    if (!World)
    {
        LastSwitchResult = TEXT("SWITCH FAILED: NO WORLD");
        return false;
    }

    APawn* TargetPawn = nullptr;
    if (TargetRole == EDSPrototypeHeroRole::Reel)
    {
        TargetPawn = ReelPawn.Get();
        if (!TargetPawn && ReelClass)
        {
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.Owner = this;
            TargetPawn = World->SpawnActor<ADSReelPrototypeCharacter>(ReelClass, GetSpawnLocationForRole(TargetRole), GetSpawnRotationForRole(), SpawnParameters);
            ReelPawn = Cast<ADSReelPrototypeCharacter>(TargetPawn);
        }
    }
    else if (TargetRole == EDSPrototypeHeroRole::Fly)
    {
        TargetPawn = FlyPawn.Get();
        if (!TargetPawn && FlyClass)
        {
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.Owner = this;
            TargetPawn = World->SpawnActor<ADSFlyPrototypeCharacter>(FlyClass, GetSpawnLocationForRole(TargetRole), GetSpawnRotationForRole(), SpawnParameters);
            FlyPawn = Cast<ADSFlyPrototypeCharacter>(TargetPawn);
        }
    }
    else
    {
        TargetPawn = LillyPawn.Get();
        if (!TargetPawn && LillyClass)
        {
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.Owner = this;
            TargetPawn = World->SpawnActor<ADSLillyPrototypeCharacter>(LillyClass, GetSpawnLocationForRole(TargetRole), GetSpawnRotationForRole(), SpawnParameters);
            LillyPawn = Cast<ADSLillyPrototypeCharacter>(TargetPawn);
        }
    }

    if (!TargetPawn)
    {
        LastSwitchResult = TEXT("SWITCH FAILED: MISSING HERO");
        ShowSwitchMessage();
        return false;
    }

    if (GetPawn() == TargetPawn)
    {
        ActiveRole = TargetRole;
        LastSwitchResult = FString::Printf(TEXT("ACTIVE: %s"), *GetActiveHeroLabel());
        ShowSwitchMessage();
        return true;
    }

    Possess(TargetPawn);
    ActiveRole = TargetRole;
    ++SwitchCount;
    LastSwitchResult = FString::Printf(TEXT("SWITCHED: %s"), *GetActiveHeroLabel());
    ShowSwitchMessage();
    return true;
}

FVector ADSPrototypePlayerController::GetSpawnLocationForRole(EDSPrototypeHeroRole TargetRole) const
{
    const APawn* CurrentPawn = GetPawn();
    if (!CurrentPawn)
    {
        return FVector::ZeroVector;
    }

    float SideOffset = -160.0f;
    if (TargetRole == EDSPrototypeHeroRole::Fly)
    {
        SideOffset = 160.0f;
    }
    else if (TargetRole == EDSPrototypeHeroRole::Lilly)
    {
        SideOffset = -260.0f;
    }

    return CurrentPawn->GetActorLocation() + (CurrentPawn->GetActorRightVector() * SideOffset);
}

FRotator ADSPrototypePlayerController::GetSpawnRotationForRole() const
{
    if (const APawn* CurrentPawn = GetPawn())
    {
        return CurrentPawn->GetActorRotation();
    }

    return FRotator::ZeroRotator;
}

void ADSPrototypePlayerController::ShowSwitchMessage() const
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(82031, 2.0f, FColor::Cyan, GetSwitchStatusText());
    }
}

void ADSPrototypePlayerController::RefreshBossArenaAwareness()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        LastBossArenaStatus = TEXT("DEEP DOCK: NO WORLD");
        return;
    }

    for (TActorIterator<ADSDeepDockBossArenaActor> It(World); It; ++It)
    {
        ADSDeepDockBossArenaActor* Arena = *It;
        if (!Arena)
        {
            continue;
        }

        Arena->EvaluateBossWeakPointCombos();
        const int32 ComboCount = Arena->GetComboTriggerCount();
        if (ComboCount > LastKnownBossComboCount)
        {
            const int32 NewComboCount = ComboCount - LastKnownBossComboCount;
            LastKnownBossComboCount = ComboCount;
            NotifyPrototypeAction(FName(TEXT("HookLineSinker")), 120 * NewComboCount, 120 * NewComboCount, 3 * NewComboCount);
        }

        if (Arena->IsBossDefeated() && !bBossDefeatRewarded)
        {
            bBossDefeatRewarded = true;
            NotifyPrototypeAction(FName(TEXT("BossDefeated")), 500, 400, 5);
        }

        LastBossArenaStatus = Arena->GetArenaStatusText();
        if (!bDeepDockDiscovered)
        {
            bDeepDockDiscovered = true;
            UnlockAchievement(FName(TEXT("DEEP_DOCK_ONLINE")));
        }
        return;
    }

    LastBossArenaStatus = TEXT("DEEP DOCK: NOT DEPLOYED");
}

void ADSPrototypePlayerController::RefreshMutationEncounterAwareness()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        LastMutationStatus = TEXT("MUTATION: NO WORLD");
        return;
    }

    for (TActorIterator<ADSMutationEnemyActor> It(World); It; ++It)
    {
        ADSMutationEnemyActor* Mutation = *It;
        if (!Mutation)
        {
            continue;
        }

        Mutation->EvaluateHeroCombo();
        const int32 ComboCount = Mutation->GetComboImpactCount();
        if (ComboCount > LastKnownMutationComboCount)
        {
            const int32 NewComboCount = ComboCount - LastKnownMutationComboCount;
            LastKnownMutationComboCount = ComboCount;
            NotifyPrototypeAction(FName(TEXT("MutationCombo")), 90 * NewComboCount, 95 * NewComboCount, 2 * NewComboCount);
        }

        if (Mutation->IsDefeated() && !bMutationDefeatRewarded)
        {
            bMutationDefeatRewarded = true;
            NotifyPrototypeAction(FName(TEXT("MutationDefeated")), 160, 160, 4);
        }

        LastMutationStatus = Mutation->GetMutationStatusText();
        if (!bMutationDiscovered)
        {
            bMutationDiscovered = true;
            UnlockAchievement(FName(TEXT("MUTATION_CONTACT")));
        }
        return;
    }

    LastMutationStatus = TEXT("MUTATION: NOT DEPLOYED");
}

void ADSPrototypePlayerController::AdvanceMissionFromAction(FName ActionName)
{
    if (ActionName == FName(TEXT("CivilianRescue")))
    {
        ++ExtractionSecuredCount;
        MissionObjectiveText = TEXT("OBJECTIVE: Civilian secured, push to extraction");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress, 0.32f);
    }
    else if (ActionName == FName(TEXT("BoatTow")))
    {
        MissionObjectiveText = TEXT("OBJECTIVE: Rescue boat tethered, keep the lane clear");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress, 0.42f);
    }
    else if (ActionName == FName(TEXT("GrapplePull")) || ActionName == FName(TEXT("ReelCast")))
    {
        MissionObjectiveText = TEXT("OBJECTIVE: Reel line active, secure the target");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress, 0.2f);
    }
    else if (ActionName == FName(TEXT("FlyMark")))
    {
        MissionObjectiveText = TEXT("OBJECTIVE: Fly mark acquired, exploit the weak point");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress, 0.45f);
    }
    else if (ActionName == FName(TEXT("LillyBind")))
    {
        MissionObjectiveText = TEXT("OBJECTIVE: Lilly bind active, hold containment");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress, 0.48f);
    }
    else if (ActionName == FName(TEXT("BossWeakPointExposed")) || ActionName == FName(TEXT("MutationStagger")))
    {
        MissionObjectiveText = TEXT("OBJECTIVE: Weak point open, commit combo damage");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress, 0.58f);
    }
    else if (ActionName == FName(TEXT("HookLineSinker")) || ActionName == FName(TEXT("MutationCombo")))
    {
        MissionObjectiveText = TEXT("OBJECTIVE: Hook, Line & Sinker landed");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress, 0.72f);
    }
    else if (ActionName == FName(TEXT("BossDefeated")) || ActionName == FName(TEXT("MutationDefeated")))
    {
        MissionObjectiveText = TEXT("OBJECTIVE: Threat down, extract samples and civilians");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress, 0.88f);
    }
    else if (ActionName == FName(TEXT("DuctSighting")))
    {
        MissionObjectiveText = TEXT("OBJECTIVE: Duct sighting logged, do not capture");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress, 0.5f);
    }
    else if (ActionName == FName(TEXT("DuctNearCatch")) || ActionName == FName(TEXT("DuctTrace")))
    {
        MissionObjectiveText = TEXT("OBJECTIVE: Duct trace secured, legend remains uncaught");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress, 0.68f);
    }
    else if (ActionName == FName(TEXT("LineSnap")))
    {
        ++ExtractionFailureCount;
        MissionObjectiveText = TEXT("OBJECTIVE: Line snapped, recover target pressure");
        MissionObjectiveProgress = FMath::Max(MissionObjectiveProgress - 0.04f, 0.08f);
    }
}

void ADSPrototypePlayerController::RoutePrototypeCuesForAction(FName ActionName)
{
    if (ActionName == FName(TEXT("ReelCast")))
    {
        SetAudioCue(FName(TEXT("SFX_REEL_CAST")), 0.72f);
        SetVisualCue(FName(TEXT("VFX_REEL_LINE")), 0.8f);
    }
    else if (ActionName == FName(TEXT("CivilianRescue")))
    {
        SetAudioCue(FName(TEXT("SFX_RESCUE_SECURED")), 1.0f);
        SetVisualCue(FName(TEXT("VFX_EXTRACTION_MARKER")), 1.0f);
    }
    else if (ActionName == FName(TEXT("BoatTow")))
    {
        SetAudioCue(FName(TEXT("SFX_BOAT_TOW")), 0.9f);
        SetVisualCue(FName(TEXT("VFX_TOW_WAKE")), 0.85f);
    }
    else if (ActionName == FName(TEXT("GrapplePull")))
    {
        SetAudioCue(FName(TEXT("SFX_GRAPPLE_PULL")), 0.9f);
        SetVisualCue(FName(TEXT("VFX_GRAPPLE_STREAK")), 0.9f);
    }
    else if (ActionName == FName(TEXT("FlyMark")))
    {
        SetAudioCue(FName(TEXT("SFX_FLY_MARK")), 0.75f);
        SetVisualCue(FName(TEXT("VFX_SONAR_RING")), 0.85f);
    }
    else if (ActionName == FName(TEXT("LillyBind")))
    {
        SetAudioCue(FName(TEXT("SFX_LILLY_BIND")), 0.78f);
        SetVisualCue(FName(TEXT("VFX_ROOT_SNARE")), 0.9f);
    }
    else if (ActionName == FName(TEXT("BossWeakPointExposed")))
    {
        SetAudioCue(FName(TEXT("SFX_WEAKPOINT_EXPOSE")), 1.0f);
        SetVisualCue(FName(TEXT("VFX_WEAKPOINT_RETICLE")), 1.0f);
    }
    else if (ActionName == FName(TEXT("HookLineSinker")))
    {
        SetAudioCue(FName(TEXT("SFX_COMBO_IMPACT")), 1.0f);
        SetVisualCue(FName(TEXT("VFX_COMBO_FLASH")), 1.0f);
    }
    else if (ActionName == FName(TEXT("BossDefeated")))
    {
        SetAudioCue(FName(TEXT("SFX_BOSS_DOWN")), 1.0f);
        SetVisualCue(FName(TEXT("VFX_BOSS_FALLOFF")), 1.0f);
    }
    else if (ActionName == FName(TEXT("MutationStagger")))
    {
        SetAudioCue(FName(TEXT("SFX_MUTATION_STAGGER")), 0.88f);
        SetVisualCue(FName(TEXT("VFX_MUTATION_SPARK")), 0.9f);
    }
    else if (ActionName == FName(TEXT("MutationCombo")))
    {
        SetAudioCue(FName(TEXT("SFX_MUTATION_COMBO")), 1.0f);
        SetVisualCue(FName(TEXT("VFX_MUTATION_COMBO_FLASH")), 1.0f);
    }
    else if (ActionName == FName(TEXT("MutationDefeated")))
    {
        SetAudioCue(FName(TEXT("SFX_MUTATION_DOWN")), 1.0f);
        SetVisualCue(FName(TEXT("VFX_SAMPLE_SECURED")), 1.0f);
    }
    else if (ActionName == FName(TEXT("DuctSighting")))
    {
        SetAudioCue(FName(TEXT("SFX_DUCT_PING")), 0.9f);
        SetVisualCue(FName(TEXT("VFX_DUCT_WAKE")), 0.95f);
    }
    else if (ActionName == FName(TEXT("DuctNearCatch")))
    {
        SetAudioCue(FName(TEXT("SFX_DUCT_SLIP")), 1.0f);
        SetVisualCue(FName(TEXT("VFX_DUCT_TENSION_SPIKE")), 1.0f);
    }
    else if (ActionName == FName(TEXT("DuctTrace")))
    {
        SetAudioCue(FName(TEXT("SFX_DUCT_TRACE")), 0.82f);
        SetVisualCue(FName(TEXT("VFX_DUCT_TAPE_FRAGMENT")), 0.86f);
    }
    else if (ActionName == FName(TEXT("LineSnap")))
    {
        SetAudioCue(FName(TEXT("SFX_LINE_SNAP")), 0.95f);
        SetVisualCue(FName(TEXT("VFX_LINE_SNAP")), 0.95f);
    }
}

void ADSPrototypePlayerController::SetAudioCue(FName CueId, float Intensity)
{
    if (CueId.IsNone())
    {
        return;
    }

    LastAudioCueId = CueId;
    LastAudioCueIntensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
    ++AudioCueEventCount;
}

void ADSPrototypePlayerController::SetVisualCue(FName CueId, float Intensity)
{
    if (CueId.IsNone())
    {
        return;
    }

    LastVisualCueId = CueId;
    LastVisualCueIntensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
    ++VisualCueEventCount;
}

void ADSPrototypePlayerController::SetCombatFeedback(FName ActionName, const FString& FeedbackText, float FlashSeconds)
{
    if (ActionName.IsNone())
    {
        return;
    }

    LastCombatFeedbackText = FeedbackText;
    CombatFeedbackFlash = FMath::Max(0.1f, FlashSeconds);
    ++CombatFeedbackEventCount;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(82034, FlashSeconds, FColor::Orange, LastCombatFeedbackText);
    }
}

FString ADSPrototypePlayerController::GetAchievementLabel(FName AchievementId) const
{
    if (AchievementId == FName(TEXT("FIRST_CAST")))
    {
        return TEXT("First Cast");
    }
    if (AchievementId == FName(TEXT("FIRST_RESCUE")))
    {
        return TEXT("First Civilian Rescue");
    }
    if (AchievementId == FName(TEXT("FIRST_BOAT_TOW")))
    {
        return TEXT("First Boat Tow");
    }
    if (AchievementId == FName(TEXT("FIRST_GRAPPLE_PULL")))
    {
        return TEXT("First Grapple Pull");
    }
    if (AchievementId == FName(TEXT("FIRST_FLY_MARK")))
    {
        return TEXT("First Fly Mark");
    }
    if (AchievementId == FName(TEXT("FIRST_LILLY_BIND")))
    {
        return TEXT("First Lilly Bind");
    }
    if (AchievementId == FName(TEXT("DEEP_DOCK_ONLINE")))
    {
        return TEXT("Deep Dock Online");
    }
    if (AchievementId == FName(TEXT("FIRST_BOSS_EXPOSE")))
    {
        return TEXT("First Boss Weak Point Exposed");
    }
    if (AchievementId == FName(TEXT("FIRST_HOOK_LINE_SINKER")))
    {
        return TEXT("First Hook, Line & Sinker");
    }
    if (AchievementId == FName(TEXT("BOSS_DEFEATED")))
    {
        return TEXT("Deep Dock Boss Defeated");
    }
    if (AchievementId == FName(TEXT("MUTATION_CONTACT")))
    {
        return TEXT("Bayou Mutation Contact");
    }
    if (AchievementId == FName(TEXT("FIRST_MUTATION_STAGGER")))
    {
        return TEXT("First Mutation Stagger");
    }
    if (AchievementId == FName(TEXT("FIRST_MUTATION_COMBO")))
    {
        return TEXT("First Mutation Combo");
    }
    if (AchievementId == FName(TEXT("FIRST_MUTATION_DEFEATED")))
    {
        return TEXT("First Mutation Defeated");
    }
    if (AchievementId == FName(TEXT("DUCT_FIRST_SIGHTING")))
    {
        return TEXT("First Duct Sighting");
    }
    if (AchievementId == FName(TEXT("DUCT_ALMOST_HAD_HIM")))
    {
        return TEXT("Almost Had Duct");
    }
    if (AchievementId == FName(TEXT("DUCT_TAPE_EVIDENCE")))
    {
        return TEXT("Duct Tape Evidence");
    }

    return AchievementId.ToString();
}
