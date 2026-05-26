#include "DSPrototypeGameMode.h"
#include "DSReelPrototypeCharacter.h"

ADSPrototypeGameMode::ADSPrototypeGameMode()
{
    DefaultPawnClass = ADSReelPrototypeCharacter::StaticClass();
}
