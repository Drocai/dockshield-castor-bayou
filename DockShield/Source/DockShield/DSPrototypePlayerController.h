#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPrototypePlayerController.generated.h"

class ADSFlyPrototypeCharacter;
class ADSLillyPrototypeCharacter;
class ADSReelPrototypeCharacter;

UENUM(BlueprintType)
enum class EDSPrototypeHeroRole : uint8
{
    Reel,
    Fly,
    Lilly
};

UENUM(BlueprintType)
enum class EDSPrototypeVisualQuality : uint8
{
    Performance UMETA(DisplayName = "Performance"),
    Balanced UMETA(DisplayName = "Balanced"),
    Cinematic UMETA(DisplayName = "Cinematic")
};

UCLASS()
class DOCKSHIELD_API ADSPrototypePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ADSPrototypePlayerController();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Squad")
    bool SwitchToReel();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Squad")
    bool SwitchToFly();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Squad")
    bool SwitchToLilly();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Squad")
    bool CycleHero();

    UFUNCTION(BlueprintPure, Category = "DockShield|Squad")
    FString GetActiveHeroLabel() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Squad")
    FString GetSwitchStatusText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Squad")
    int32 GetSwitchCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Squad")
    AActor* GetReelPawnActor() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Squad")
    AActor* GetFlyPawnActor() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Squad")
    AActor* GetLillyPawnActor() const;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Meta")
    void NotifyPrototypeAction(FName ActionName, int32 CreditReward, int32 XPReward, int32 SampleReward);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Meta")
    bool UnlockAchievement(FName AchievementId);

    UFUNCTION(BlueprintPure, Category = "DockShield|Meta")
    int32 GetBetaCredits() const { return BetaCredits; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Meta")
    int32 GetPrototypeXP() const { return PrototypeXP; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Meta")
    int32 GetDepthSamples() const { return DepthSamples; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Meta")
    int32 GetAchievementUnlockCount() const { return UnlockedAchievements.Num(); }

    UFUNCTION(BlueprintPure, Category = "DockShield|Meta")
    FString GetEconomyStatusText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Meta")
    FString GetAchievementStatusText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Meta")
    FString GetBossArenaStatusText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Settings")
    FString GetSettingsStatusText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Settings")
    bool IsSettingsPanelOpen() const { return bSettingsPanelOpen; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Settings")
    float GetHudScale() const { return HudScale; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Settings")
    float GetGamma() const { return Gamma; }

    UFUNCTION(BlueprintPure, Category = "DockShield|Settings")
    FString GetVisualQualityLabel() const;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Settings")
    void ToggleSettingsPanel();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Settings")
    void CycleVisualQuality();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Settings")
    void IncreaseHudScale();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Settings")
    void DecreaseHudScale();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Settings")
    void IncreaseGamma();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Settings")
    void DecreaseGamma();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "DockShield|Squad")
    TSubclassOf<ADSReelPrototypeCharacter> ReelClass;

    UPROPERTY(EditDefaultsOnly, Category = "DockShield|Squad")
    TSubclassOf<ADSFlyPrototypeCharacter> FlyClass;

    UPROPERTY(EditDefaultsOnly, Category = "DockShield|Squad")
    TSubclassOf<ADSLillyPrototypeCharacter> LillyClass;

private:
    UPROPERTY()
    TWeakObjectPtr<ADSReelPrototypeCharacter> ReelPawn;

    UPROPERTY()
    TWeakObjectPtr<ADSFlyPrototypeCharacter> FlyPawn;

    UPROPERTY()
    TWeakObjectPtr<ADSLillyPrototypeCharacter> LillyPawn;

    EDSPrototypeHeroRole ActiveRole = EDSPrototypeHeroRole::Reel;
    int32 SwitchCount = 0;
    FString LastSwitchResult = TEXT("SQUAD READY");

    int32 BetaCredits = 120;
    int32 PrototypeXP = 0;
    int32 DepthSamples = 0;
    TSet<FName> UnlockedAchievements;
    FString LastEconomyEvent = TEXT("No rewards yet");
    FString LastAchievementText = TEXT("None unlocked");
    FString LastBossArenaStatus = TEXT("DEEP DOCK: NOT DEPLOYED");
    bool bDeepDockDiscovered = false;

    bool bSettingsPanelOpen = false;
    float HudScale = 1.0f;
    float Gamma = 1.0f;
    EDSPrototypeVisualQuality VisualQuality = EDSPrototypeVisualQuality::Balanced;
    float BossArenaScanAccumulator = 0.0f;

    void HandleSwitchToReelInput();
    void HandleSwitchToFlyInput();
    void HandleSwitchToLillyInput();
    void HandleCycleHeroInput();
    void HandleToggleSettingsInput();
    void RegisterCurrentPawn();
    bool SwitchToRole(EDSPrototypeHeroRole TargetRole);
    FVector GetSpawnLocationForRole(EDSPrototypeHeroRole TargetRole) const;
    FRotator GetSpawnRotationForRole() const;
    void ShowSwitchMessage() const;
    void RefreshBossArenaAwareness();
    FString GetAchievementLabel(FName AchievementId) const;
};
