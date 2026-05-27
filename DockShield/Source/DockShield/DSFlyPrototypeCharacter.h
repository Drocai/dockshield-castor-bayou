#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DSFlyPrototypeCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UStaticMeshComponent;
class USpringArmComponent;
class UDSTargetableComponent;
struct FInputActionValue;

UCLASS()
class DOCKSHIELD_API ADSFlyPrototypeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ADSFlyPrototypeCharacter();

    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Fly")
    int32 SonarPulse();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Fly")
    bool ExecuteFlyMarkOnTarget(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Fly")
    void TryFlyMark();

    UFUNCTION(BlueprintPure, Category = "DockShield|Fly")
    AActor* GetCurrentReconTargetActor() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Fly")
    FString GetCurrentReconPrompt() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Fly")
    bool IsCurrentReconTargetMarkValid() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Fly")
    bool IsAiming() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Fly")
    float GetSonarRange() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Fly")
    float GetSonarConeDegrees() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Fly")
    int32 GetLastSonarHitCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Fly")
    int32 GetMarkedTargetCount() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Fly")
    FString GetLastReconResult() const;

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
    TObjectPtr<UStaticMeshComponent> FlyLeftWingMesh;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Prototype Visuals")
    TObjectPtr<UStaticMeshComponent> FlyRightWingMesh;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Prototype Visuals")
    TObjectPtr<UStaticMeshComponent> FlyVisorBeaconMesh;

    UPROPERTY(VisibleAnywhere, Category = "DockShield|Prototype Visuals")
    TObjectPtr<UStaticMeshComponent> FlyProbeRodMesh;

    UPROPERTY()
    TWeakObjectPtr<AActor> CurrentReconTarget;

    bool bIsAiming = false;
    float SonarRange = 1600.0f;
    float SonarConeDegrees = 72.0f;
    int32 LastSonarHitCount = 0;
    int32 MarkedTargetCount = 0;
    FString LastReconResult = TEXT("FLY SENSE READY");

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void HandleSonarPulse();
    void StartAim();
    void StopAim();
    AActor* FindBestReconTarget() const;
    UDSTargetableComponent* GetTargetableComponent(AActor* Actor) const;
    bool CanFlyMark(AActor* Actor) const;
    bool IsInsideSonarCone(AActor* Actor, float MinDot) const;
    void ApplyPrototypeVisualStyle();
    void DrawSonarDebug() const;
    void ShowDebugMessage(const FString& Message, const FColor& Color) const;
};
