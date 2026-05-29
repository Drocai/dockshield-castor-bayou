#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSDeepDockBossArenaActor.generated.h"

UCLASS()
class DOCKSHIELD_API ADSDeepDockBossArenaActor : public AActor
{
	GENERATED_BODY()

public:
	ADSDeepDockBossArenaActor();

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	FString GetArenaStatusText() const;

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	float GetBossIntegrity() const { return BossIntegrity; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	float GetLabCoreIntegrity() const { return LabCoreIntegrity; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	float GetThreatLevel() const { return ThreatLevel; }

	UFUNCTION(BlueprintPure, Category = "DockShield|Boss Arena")
	bool IsArenaArmed() const { return bArenaArmed; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena")
	FString ArenaName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena")
	FString BossName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BossIntegrity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float LabCoreIntegrity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ThreatLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DockShield|Boss Arena")
	bool bArenaArmed;
};
