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

    if (ActionName == FName(TEXT("ReelCast")))
    {
        UnlockAchievement(FName(TEXT("FIRST_CAST")));
    }
    else if (ActionName == FName(TEXT("CivilianRescue")))
    {
        UnlockAchievement(FName(TEXT("FIRST_RESCUE")));
    }
    else if (ActionName == FName(TEXT("BoatTow")))
    {
        UnlockAchievement(FName(TEXT("FIRST_BOAT_TOW")));
    }
    else if (ActionName == FName(TEXT("GrapplePull")))
    {
        UnlockAchievement(FName(TEXT("FIRST_GRAPPLE_PULL")));
    }
    else if (ActionName == FName(TEXT("FlyMark")))
    {
        UnlockAchievement(FName(TEXT("FIRST_FLY_MARK")));
    }
    else if (ActionName == FName(TEXT("LillyBind")))
    {
        UnlockAchievement(FName(TEXT("FIRST_LILLY_BIND")));
    }
    else if (ActionName == FName(TEXT("BossWeakPointExposed")))
    {
        UnlockAchievement(FName(TEXT("FIRST_BOSS_EXPOSE")));
    }
    else if (ActionName == FName(TEXT("HookLineSinker")))
    {
        UnlockAchievement(FName(TEXT("FIRST_HOOK_LINE_SINKER")));
    }
    else if (ActionName == FName(TEXT("BossDefeated")))
    {
        UnlockAchievement(FName(TEXT("BOSS_DEFEATED")));
    }
    else if (ActionName == FName(TEXT("MutationStagger")))
    {
        UnlockAchievement(FName(TEXT("FIRST_MUTATION_STAGGER")));
    }
    else if (ActionName == FName(TEXT("MutationCombo")))
    {
        UnlockAchievement(FName(TEXT("FIRST_MUTATION_COMBO")));
    }
    else if (ActionName == FName(TEXT("MutationDefeated")))
    {
        UnlockAchievement(FName(TEXT("FIRST_MUTATION_DEFEATED")));
    }
    else if (ActionName == FName(TEXT("DuctSighting")))
    {
        ++DuctSightings;
        UnlockAchievement(FName(TEXT("DUCT_FIRST_SIGHTING")));
    }
    else if (ActionName == FName(TEXT("DuctNearCatch")))
    {
        ++DuctNearCatchCount;
        ++DuctTapeFragments;
        UnlockAchievement(FName(TEXT("DUCT_ALMOST_HAD_HIM")));
        if (DuctTapeFragments >= 3)
        {
            UnlockAchievement(FName(TEXT("DUCT_TAPE_EVIDENCE")));
        }
    }
    else if (ActionName == FName(TEXT("DuctTrace")))
    {
        ++DuctTapeFragments;
        UnlockAchievement(FName(TEXT("DUCT_TAPE_EVIDENCE")));
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
