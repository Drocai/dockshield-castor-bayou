#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DSLillyPrototypeCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UStaticMeshComponent;
class USpringArmComponent;
class UDSTargetableComponent;
struct FInputActionValue;

UCLASS()
class DOCKSHIELD_API ADSLillyPrototypeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ADSLillyPrototypeCharacter();

    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Lilly")
    int32 SwampPulse();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Lilly")
    bool ExecuteLillyBindOnTarget(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Lilly")
    void TryLillyBind();

    UFUNCTION(BlueprintPure, Category = "DockShield|Lilly")
    AActor* GetCurrentBindTargetActor() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Lilly")
    FString GetCurrentBindPrompt() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Lilly")
    bool IsCurrentBindTargetValid() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Lilly")
    bool IsAiming() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Lilly")
    float GetBindRange() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Lilly")
    float GetRootRadius() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Lilly")
    int32 GetLastSwampPulseHitCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Lilly")
    int32 GetBoundTargetCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Lilly")
    FString GetLastBindResult() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Visual")
    int32 GetPrototypeVisualKitComponentCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Visual")
    FString GetPrototypeVisualProfile() const;

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

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Prototype Visuals")
    TObjectPtr<UStaticMeshComponent> LillyRootStaffMesh;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Prototype Visuals")
    TObjectPtr<UStaticMeshComponent> LillyVineLashMesh;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Prototype Visuals")
    TObjectPtr<UStaticMeshComponent> LillyShoulderBloomMesh;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Prototype Visuals")
    TObjectPtr<UStaticMeshComponent> LillySwampCrownMesh;

    UPROPERTY()
    TWeakObjectPtr<AActor> CurrentBindTarget;

    bool bIsAiming = false;
    float BindRange = 1450.0f;
    float RootRadius = 420.0f;
    int32 LastSwampPulseHitCount = 0;
    int32 BoundTargetCount = 0;
    FString LastBindResult = TEXT("SWAMP BOND READY");

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void HandleSwampPulse();
    void StartAim();
    void StopAim();
    AActor* FindBestBindTarget() const;
    UDSTargetableComponent* GetTargetableComponent(AActor* Actor) const;
    bool CanLillyBind(AActor* Actor) const;
    bool IsInsideRootFocus(AActor* Actor, float MinDot) const;
    float GetEffectiveBindRange() const;
    void ApplyPrototypeVisualStyle();
    void DrawRootDebug() const;
    void ShowDebugMessage(const FString& Message, const FColor& Color) const;
};
