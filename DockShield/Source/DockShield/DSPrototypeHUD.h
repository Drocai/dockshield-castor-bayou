#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DSPrototypeHUD.generated.h"

class ADSFlyPrototypeCharacter;

UCLASS()
class DOCKSHIELD_API ADSPrototypeHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

private:
    void DrawFlyHUD(const ADSFlyPrototypeCharacter* FlyCharacter, float ScreenWidth, float ScreenHeight, float CenterX, float CenterY);
    void DrawReticle(float CenterX, float CenterY, const FLinearColor& Color);
    void DrawPanel(float X, float Y, float Width, float Height, const FLinearColor& Color);
    void DrawBar(float X, float Y, float Width, float Height, float Value, const FLinearColor& FillColor);
};
