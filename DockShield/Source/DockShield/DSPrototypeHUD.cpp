#include "DSPrototypeHUD.h"
#include "DSReelPrototypeCharacter.h"
#include "Engine/Canvas.h"
#include "GameFramework/PlayerController.h"

void ADSPrototypeHUD::DrawHUD()
{
    Super::DrawHUD();

    if (!Canvas)
    {
        return;
    }

    const float ScreenWidth = Canvas->SizeX;
    const float ScreenHeight = Canvas->SizeY;
    const float CenterX = ScreenWidth * 0.5f;
    const float CenterY = ScreenHeight * 0.5f;

    const ADSReelPrototypeCharacter* ReelCharacter = nullptr;
    if (APlayerController* PlayerController = GetOwningPlayerController())
    {
        ReelCharacter = Cast<ADSReelPrototypeCharacter>(PlayerController->GetPawn());
    }

    const bool bHasTarget = ReelCharacter && ReelCharacter->GetCurrentTargetActor();
    const bool bValidTarget = ReelCharacter && ReelCharacter->IsCurrentTargetReelPullValid();
    FString Prompt = TEXT("DockShield Reel v0: acquire target");
    if (ReelCharacter)
    {
        Prompt = ReelCharacter->GetCurrentTargetPrompt();
    }
    if (Prompt.Len() > 72)
    {
        Prompt = Prompt.Left(69) + TEXT("...");
    }

    const FLinearColor ValidColor(0.05f, 0.95f, 0.35f, 1.0f);
    const FLinearColor InvalidColor(1.0f, 0.55f, 0.08f, 1.0f);
    const FLinearColor NeutralColor(0.84f, 0.92f, 1.0f, 1.0f);
    const FLinearColor HudColor = bValidTarget ? ValidColor : (bHasTarget ? InvalidColor : NeutralColor);
    const FLinearColor PanelColor(0.0f, 0.0f, 0.0f, 0.48f);

    DrawReticle(CenterX, CenterY, HudColor);

    DrawPanel(28.0f, 28.0f, 300.0f, 118.0f, PanelColor);
    DrawText(TEXT("THE REEL"), FLinearColor(0.85f, 0.95f, 1.0f, 1.0f), 44.0f, 42.0f, nullptr, 1.45f);
    DrawText(TEXT("PUBLIC HERO"), FLinearColor(0.2f, 0.7f, 1.0f, 1.0f), 46.0f, 74.0f, nullptr, 0.82f);
    DrawText(TEXT("OBJECTIVE: Test Pull / Rescue"), FLinearColor(0.75f, 1.0f, 0.78f, 1.0f), 44.0f, 106.0f, nullptr, 0.78f);

    DrawPanel(ScreenWidth - 336.0f, 28.0f, 308.0f, 96.0f, PanelColor);
    DrawText(TEXT("M_TEST_TARGETING"), FLinearColor(0.86f, 0.86f, 0.78f, 1.0f), ScreenWidth - 318.0f, 44.0f, nullptr, 0.82f);
    DrawText(TEXT("TARGETING RANGE"), FLinearColor(1.0f, 0.42f, 0.35f, 1.0f), ScreenWidth - 318.0f, 72.0f, nullptr, 0.75f);

    const float PromptWidth = 560.0f;
    const float PromptHeight = 56.0f;
    DrawPanel(CenterX - (PromptWidth * 0.5f), ScreenHeight - 132.0f, PromptWidth, PromptHeight, PanelColor);
    DrawText(Prompt, HudColor, CenterX - (PromptWidth * 0.5f) + 18.0f, ScreenHeight - 115.0f, nullptr, 0.86f);

    DrawPanel(ScreenWidth - 250.0f, ScreenHeight - 126.0f, 222.0f, 58.0f, PanelColor);
    DrawText(TEXT("REEL PULL"), FLinearColor(0.85f, 0.85f, 0.78f, 1.0f), ScreenWidth - 232.0f, ScreenHeight - 111.0f, nullptr, 0.78f);
    DrawText(TEXT("E"), HudColor, ScreenWidth - 82.0f, ScreenHeight - 116.0f, nullptr, 1.2f);
}

void ADSPrototypeHUD::DrawReticle(float CenterX, float CenterY, const FLinearColor& Color)
{
    constexpr float InnerGap = 7.0f;
    constexpr float Outer = 18.0f;

    DrawLine(CenterX - Outer, CenterY, CenterX - InnerGap, CenterY, Color, 2.0f);
    DrawLine(CenterX + InnerGap, CenterY, CenterX + Outer, CenterY, Color, 2.0f);
    DrawLine(CenterX, CenterY - Outer, CenterX, CenterY - InnerGap, Color, 2.0f);
    DrawLine(CenterX, CenterY + InnerGap, CenterX, CenterY + Outer, Color, 2.0f);
}

void ADSPrototypeHUD::DrawPanel(float X, float Y, float Width, float Height, const FLinearColor& Color)
{
    DrawRect(Color, X, Y, Width, Height);
    DrawLine(X, Y, X + Width, Y, FLinearColor(0.7f, 0.78f, 0.68f, 0.35f), 1.0f);
    DrawLine(X, Y + Height, X + Width, Y + Height, FLinearColor(0.7f, 0.78f, 0.68f, 0.25f), 1.0f);
}
