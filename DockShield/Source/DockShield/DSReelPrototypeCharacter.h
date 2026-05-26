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

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void TryReelPull();
    AActor* FindBestTarget() const;
    UDSTargetableComponent* GetTargetableComponent(AActor* Actor) const;
    bool CanReelPull(AActor* Actor) const;
    void ShowDebugMessage(const FString& Message, const FColor& Color) const;
};
