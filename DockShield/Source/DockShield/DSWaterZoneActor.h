#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSWaterZoneActor.generated.h"

class UBoxComponent;

UCLASS(Blueprintable)
class DOCKSHIELD_API ADSWaterZoneActor : public AActor
{
    GENERATED_BODY()

public:
    ADSWaterZoneActor();

    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Water")
    TObjectPtr<UBoxComponent> WaterBounds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Water")
    float WaterSurfaceZ = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Water")
    float WaistDeepDepth = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Water")
    float DeepWaterDepth = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Water")
    float BoatOperationalDepth = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Water")
    FVector CurrentDirection = FVector(1.0f, 0.0f, 0.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Water")
    float CurrentSpeed = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DockShield|Water")
    bool bToxic = false;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    bool ContainsWorldLocation(FVector WorldLocation) const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    float GetDepthAtLocation(FVector WorldLocation) const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    float GetMovementScaleAtLocation(FVector WorldLocation) const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    bool CanBoatOperateAtLocation(FVector WorldLocation) const;

    UFUNCTION(BlueprintPure, Category = "DockShield|Water")
    FVector GetCurrentVelocity() const;
};
