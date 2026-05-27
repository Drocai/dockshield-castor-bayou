#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSPrototypeBoatActor.generated.h"

class UDSTargetableComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class DOCKSHIELD_API ADSPrototypeBoatActor : public AActor
{
    GENERATED_BODY()

public:
    ADSPrototypeBoatActor();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Boat")
    TObjectPtr<UStaticMeshComponent> BoatMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Boat")
    TObjectPtr<UDSTargetableComponent> TargetableComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    float CurrentWaterDepth = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    float RequiredWaterDepth = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    float ReelTowStepDistance = 280.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    float PilotSpeed = 420.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    float CurrentDriftScale = 0.42f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    float FloodDragScale = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    bool bUseWaterZoneForces = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    FVector SeatOffset = FVector(0.0f, 0.0f, 62.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    FVector ExitOffset = FVector(0.0f, 180.0f, 80.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    float BobAmplitude = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    float BobSpeed = 2.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    bool bOccupied = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Boat")
    bool bAnchored = false;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Boat")
    void SetWaterDepth(float Depth);

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    bool CanFloat() const;

    UFUNCTION(BlueprintCallable, Category = "DockShield|Boat")
    bool BoardBoat(AActor* Rider);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Boat")
    void ExitBoat();

    UFUNCTION(BlueprintCallable, Category = "DockShield|Boat")
    bool ApplyReelTowFrom(FVector PullOrigin, float Strength);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Boat")
    bool ApplyPilotInput(FVector2D MoveInput, float ControllerYawDegrees, float DeltaSeconds);

    UFUNCTION(BlueprintCallable, Category = "DockShield|Boat")
    bool ApplyWaterForces(FVector CurrentVelocity, float FloodPressure, float DeltaSeconds);

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    FString GetBoatStateText() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    float GetLastTowDistance() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    float GetLastPilotDistance() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    float GetLastDriftDistance() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    float GetFloodPressure() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    FVector GetWaterCurrentVelocity() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    float GetWaterCurrentSpeed() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    FVector GetSeatWorldLocation() const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Boat")
    FVector GetExitWorldLocation() const;

private:
    TWeakObjectPtr<AActor> Occupant;
    float BaseZ = 0.0f;
    float RunningTime = 0.0f;
    float LastTowDistance = 0.0f;
    float LastPilotDistance = 0.0f;
    float LastDriftDistance = 0.0f;
    float FloodPressure = 0.0f;
    FVector WaterCurrentVelocity = FVector::ZeroVector;

    void UpdateWaterZoneForces(float DeltaSeconds);
    float GetFloodDragMultiplier() const;
};
