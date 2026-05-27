#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPrototypePlayerController.generated.h"

class ADSFlyPrototypeCharacter;
class ADSReelPrototypeCharacter;

UENUM(BlueprintType)
enum class EDSPrototypeHeroRole : uint8
{
    Reel,
    Fly
};

UCLASS()
class DOCKSHIELD_API ADSPrototypePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ADSPrototypePlayerController();

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Squad")
    bool SwitchToReel();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Squad")
    bool SwitchToFly();

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

protected:
    UPROPERTY(EditDefaultsOnly, Category = "DockShield|Squad")
    TSubclassOf<ADSReelPrototypeCharacter> ReelClass;

    UPROPERTY(EditDefaultsOnly, Category = "DockShield|Squad")
    TSubclassOf<ADSFlyPrototypeCharacter> FlyClass;

private:
    UPROPERTY()
    TWeakObjectPtr<ADSReelPrototypeCharacter> ReelPawn;

    UPROPERTY()
    TWeakObjectPtr<ADSFlyPrototypeCharacter> FlyPawn;

    EDSPrototypeHeroRole ActiveRole = EDSPrototypeHeroRole::Reel;
    int32 SwitchCount = 0;
    FString LastSwitchResult = TEXT("SQUAD READY");

    void HandleSwitchToReelInput();
    void HandleSwitchToFlyInput();
    void HandleCycleHeroInput();
    void RegisterCurrentPawn();
    bool SwitchToRole(EDSPrototypeHeroRole TargetRole);
    FVector GetSpawnLocationForRole(EDSPrototypeHeroRole TargetRole) const;
    FRotator GetSpawnRotationForRole() const;
    void ShowSwitchMessage() const;
};
