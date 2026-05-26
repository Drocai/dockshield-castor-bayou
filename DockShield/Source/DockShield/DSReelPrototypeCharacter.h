#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DSReelPrototypeCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UDSTargetableComponent;
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

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    float GetCurrentTargetDistance() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    float GetLineTension() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    FString GetLastReelResult() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    int32 GetGrapplePullCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Reel")
    int32 GetCivilianRescueCount() const;

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

    bool bIsAiming = false;
    float CurrentTargetDistance = 0.0f;
    float LineTension = 0.0f;
    FString LastReelResult = TEXT("READY");
    int32 GrapplePullCount = 0;
    int32 CivilianRescueCount = 0;
    FVector LastReelFeedbackStart = FVector::ZeroVector;
    FVector LastReelFeedbackEnd = FVector::ZeroVector;
    FColor LastReelFeedbackColor = FColor::Cyan;
    float ReelFeedbackTimeRemaining = 0.0f;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartAim();
    void StopAim();
    AActor* FindBestTarget() const;
    UDSTargetableComponent* GetTargetableComponent(AActor* Actor) const;
    float GetTargetInteractionRange(AActor* Actor) const;
    void UpdateTargetMetrics(AActor* Actor);
    bool CanReelPull(AActor* Actor) const;
    void StartReelFeedback(AActor* Target, const FColor& Color);
    void DrawReelFeedback(float DeltaSeconds);
    void ShowDebugMessage(const FString& Message, const FColor& Color) const;
};
