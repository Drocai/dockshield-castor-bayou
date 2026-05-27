#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DSReelPrototypeCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UDSTargetableComponent;
class ADSPrototypeBoatActor;
struct FInputActionValue;

UCLASS()
class DOCKSHIELD_API ADSReelPrototypeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ADSReelPrototypeCharacter();

    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    AActor* GetCurrentTargetActor() const;
    FString GetCurrentTargetPrompt() const;
    bool IsCurrentTargetReelPullValid() const;
    bool IsAiming() const;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Reel")
    bool ExecuteReelActionOnTarget(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Reel")
    void TryReelPull();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Reel")
    void TryCastReelLine();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Reel")
    bool CastReelLineAtTarget(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Reel")
    void StartReelHold();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Reel")
    void StopReelHold();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Reel")
    void DetachReelLine();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Reel")
    void UpdateReelLine(float DeltaSeconds);

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    float GetCurrentTargetDistance() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    float GetLineTension() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    FString GetLastReelResult() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    bool IsReelLineAttached() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    bool IsReelHoldActive() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    FString GetReelLineStateText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    AActor* GetAttachedReelTarget() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    int32 GetReelSnapCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    int32 GetGrapplePullCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    int32 GetCivilianRescueCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    int32 GetBoatTowCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    float GetCurrentWaterDepth() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    float GetWaterMovementScale() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    bool IsInBoatableWater() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    float GetCurrentWaterPressure() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    FVector GetCurrentWaterVelocity() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    float GetCurrentWaterSpeed() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    FString GetWaterStatusText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Mission")
    FString GetPrototypeObjectiveText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Mission")
    float GetPrototypeObjectiveProgress() const;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Boat")
    bool TryBoardOrExitBoat();

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    bool IsBoardedBoat() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    FString GetBoatStatusText() const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "DockShield|Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY()
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY()
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY()
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY()
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY()
    TWeakObjectPtr<AActor> CurrentTarget;

    UPROPERTY()
    TWeakObjectPtr<AActor> AttachedReelTarget;

    UPROPERTY()
    TWeakObjectPtr<ADSPrototypeBoatActor> BoardedBoat;

    bool bIsAiming = false;
    bool bIsBoardedBoat = false;
    bool bReelHoldActive = false;
    float CurrentTargetDistance = 0.0f;
    float LineTension = 0.0f;
    float RawLineTension = 0.0f;
    FString LastReelResult = TEXT("READY");
    FString ReelLineStateText = TEXT("IDLE");
    int32 GrapplePullCount = 0;
    int32 CivilianRescueCount = 0;
    int32 BoatTowCount = 0;
    int32 ReelSnapCount = 0;
    bool bInWaterZone = false;
    bool bInBoatableWater = false;
    float CurrentWaterDepth = 0.0f;
    float WaterMovementScale = 1.0f;
    float CurrentWaterPressure = 0.0f;
    FVector CurrentWaterVelocity = FVector::ZeroVector;
    FString CurrentWaterStateText = TEXT("DRY");
    FVector LastReelFeedbackStart = FVector::ZeroVector;
    FVector LastReelFeedbackEnd = FVector::ZeroVector;
    FColor LastReelFeedbackColor = FColor::Cyan;
    float ReelFeedbackTimeRemaining = 0.0f;
    float BoardInteractionRange = 360.0f;
    float ReelTensionBuildRate = 0.42f;
    float ReelTensionReleaseRate = 0.28f;
    float ReelStrainThreshold = 0.62f;
    float ReelDangerThreshold = 0.88f;
    float ReelSnapThreshold = 1.18f;
    float ReelSelfPullAcceleration = 1550.0f;
    float ReelTargetPullSpeed = 560.0f;
    float ReelBoatTowSpeed = 640.0f;
    float ReelRescueCompleteDistance = 220.0f;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartAim();
    void StopAim();
    void HandleBoardOrExitInput();
    AActor* FindBestTarget() const;
    UDSTargetableComponent* GetTargetableComponent(AActor* Actor) const;
    float GetTargetInteractionRange(AActor* Actor) const;
    void UpdateTargetMetrics(AActor* Actor);
    void UpdateWaterState(float DeltaSeconds);
    void ApplyMovementSpeed();
    void ApplyWaterCurrent(float DeltaSeconds);
    bool CanReelPull(AActor* Actor) const;
    void ApplyContinuousReelPull(AActor* Target, float DeltaSeconds);
    void DetachReelLineInternal(const FString& ResultText, bool bWasSnap);
    void RefreshReelLineState();
    FColor GetReelLineStateColor() const;
    void ExitBoardedBoat();
    void StartReelFeedback(AActor* Target, const FColor& Color);
    void DrawReelFeedback(float DeltaSeconds);
    void ShowDebugMessage(const FString& Message, const FColor& Color) const;
};
