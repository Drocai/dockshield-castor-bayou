#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "DSTargetableInterface.h"
#include "DSTargetActor.generated.h"

class UDSTargetableComponent;

UCLASS(Blueprintable)
class DOCKSHIELD_API ADSTargetActor : public AStaticMeshActor, public IDSTargetableInterface
{
    GENERATED_BODY()

public:
    ADSTargetActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DockShield|Targeting")
    TObjectPtr<UDSTargetableComponent> TargetableComponent;

    virtual UDSTargetableComponent* GetDockShieldTargetableComponent_Implementation() const override;
};
