#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DSTargetableInterface.generated.h"

class UDSTargetableComponent;

UINTERFACE(BlueprintType)
class DOCKSHIELD_API UDSTargetableInterface : public UInterface
{
    GENERATED_BODY()
};

class DOCKSHIELD_API IDSTargetableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DockShield|Targeting")
    UDSTargetableComponent* GetDockShieldTargetableComponent() const;
};
