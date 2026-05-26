#include "DSPrototypeGameMode.h"
#include "DSPrototypeHUD.h"
#include "DSReelPrototypeCharacter.h"

ADSPrototypeGameMode::ADSPrototypeGameMode()
{
    DefaultPawnClass = ADSReelPrototypeCharacter::StaticClass();
    HUDClass = ADSPrototypeHUD::StaticClass();
}
