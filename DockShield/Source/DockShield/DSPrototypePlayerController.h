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

    void HandleSwitchToReelInput();
    void HandleSwitchToFlyInput();
    void HandleSwitchToLillyInput();
    void HandleCycleHeroInput();
    void RegisterCurrentPawn();
    bool SwitchToRole(EDSPrototypeHeroRole TargetRole);
    FVector GetSpawnLocationForRole(EDSPrototypeHeroRole TargetRole) const;
    FRotator GetSpawnRotationForRole() const;
    void ShowSwitchMessage() const;
};
