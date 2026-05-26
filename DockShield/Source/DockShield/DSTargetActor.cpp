#include "DSTargetActor.h"
#include "DSTargetableComponent.h"

ADSTargetActor::ADSTargetActor()
{
    TargetableComponent = CreateDefaultSubobject<UDSTargetableComponent>(TEXT("TargetableComponent"));
    Tags.AddUnique(TEXT("DockShieldTarget"));
}

UDSTargetableComponent* ADSTargetActor::GetDockShieldTargetableComponent_Implementation() const
{
    return TargetableComponent;
}
