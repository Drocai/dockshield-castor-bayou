#pragma once

#include "DSTargetTypes.generated.h"

UENUM(BlueprintType)
enum class EDSTargetType : uint8
{
    Unknown UMETA(DisplayName = "Unknown"),
    GrapplePoint UMETA(DisplayName = "Grapple Point"),
    Civilian UMETA(DisplayName = "Civilian"),
    Hazard UMETA(DisplayName = "Hazard"),
    Enemy UMETA(DisplayName = "Enemy"),
    WeakPoint UMETA(DisplayName = "Weak Point"),
    Object UMETA(DisplayName = "Object")
};
