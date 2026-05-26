#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DSPrototypeHUD.generated.h"

UCLASS()
class DOCKSHIELD_API ADSPrototypeHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

private:
    void DrawReticle(float CenterX, float CenterY, const FLinearColor& Color);
    void DrawPanel(float X, float Y, float Width, float Height, const FLinearColor& Color);
};
