#include "DSPrototypeGameMode.h"
#include "DSPrototypeHUD.h"
#include "DSPrototypePlayerController.h"
#include "DSReelPrototypeCharacter.h"

ADSPrototypeGameMode::ADSPrototypeGameMode()
{
    DefaultPawnClass = ADSReelPrototypeCharacter::StaticClass();
    PlayerControllerClass = ADSPrototypePlayerController::StaticClass();
    HUDClass = ADSPrototypeHUD::StaticClass();
}
