#pragma once

#include "CoreMinimal.h"
#include "DSHeroThemeTypes.generated.h"

UENUM(BlueprintType)
enum class EDSHeroTheme : uint8
{
    Reel UMETA(DisplayName = "The Reel"),
    Fly UMETA(DisplayName = "The Fly"),
    Lilly UMETA(DisplayName = "Lilly Loch")
};
