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
    const bool bAiming = ReelCharacter && ReelCharacter->IsAiming();
    const float TargetDistance = ReelCharacter ? ReelCharacter->GetCurrentTargetDistance() : 0.0f;
    const float LineTension = ReelCharacter ? ReelCharacter->GetLineTension() : 0.0f;
    const int32 GrapplePullCount = ReelCharacter ? ReelCharacter->GetGrapplePullCount() : 0;
    const int32 CivilianRescueCount = ReelCharacter ? ReelCharacter->GetCivilianRescueCount() : 0;
    const int32 BoatTowCount = ReelCharacter ? ReelCharacter->GetBoatTowCount() : 0;
    const float WaterDepth = ReelCharacter ? ReelCharacter->GetCurrentWaterDepth() : 0.0f;
    const float WaterMovementScale = ReelCharacter ? ReelCharacter->GetWaterMovementScale() : 1.0f;
    const bool bBoatableWater = ReelCharacter && ReelCharacter->IsInBoatableWater();
    const bool bBoardedBoat = ReelCharacter && ReelCharacter->IsBoardedBoat();
    FString Prompt = TEXT("DockShield Reel v0: acquire target");
    FString LastReelResult = TEXT("READY");
    FString BoatStatus = TEXT("NO BOAT TARGET");
    if (ReelCharacter)
    {
        Prompt = ReelCharacter->GetCurrentTargetPrompt();
        LastReelResult = ReelCharacter->GetLastReelResult();
        BoatStatus = ReelCharacter->GetBoatStatusText();
    }
    if (Prompt.Len() > 72)
    {
        Prompt = Prompt.Left(69) + TEXT("...");
    }
    if (LastReelResult.Len() > 32)
    {
        LastReelResult = LastReelResult.Left(29) + TEXT("...");
    }
    if (BoatStatus.Len() > 42)
    {
        BoatStatus = BoatStatus.Left(39) + TEXT("...");
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
    DrawText(bBoardedBoat ? TEXT("BOAT CONTROL") : (bAiming ? TEXT("AIM MODE") : TEXT("MOVE MODE")), bBoardedBoat ? ValidColor : (bAiming ? ValidColor : NeutralColor), 188.0f, 74.0f, nullptr, 0.72f);
    DrawText(TEXT("OBJECTIVE: Test Pull / Rescue"), FLinearColor(0.75f, 1.0f, 0.78f, 1.0f), 44.0f, 106.0f, nullptr, 0.78f);

    DrawPanel(ScreenWidth - 336.0f, 28.0f, 308.0f, 96.0f, PanelColor);
    DrawText(TEXT("M_TEST_TARGETING"), FLinearColor(0.86f, 0.86f, 0.78f, 1.0f), ScreenWidth - 318.0f, 44.0f, nullptr, 0.82f);
    DrawText(TEXT("TARGETING RANGE"), FLinearColor(1.0f, 0.42f, 0.35f, 1.0f), ScreenWidth - 318.0f, 72.0f, nullptr, 0.75f);

    const float PromptWidth = 560.0f;
    const float PromptHeight = 56.0f;
    DrawPanel(CenterX - (PromptWidth * 0.5f), ScreenHeight - 132.0f, PromptWidth, PromptHeight, PanelColor);
    DrawText(Prompt, HudColor, CenterX - (PromptWidth * 0.5f) + 18.0f, ScreenHeight - 115.0f, nullptr, 0.86f);

    DrawPanel(ScreenWidth - 250.0f, ScreenHeight - 154.0f, 222.0f, 86.0f, PanelColor);
    DrawText(TEXT("RMB AIM"), FLinearColor(0.85f, 0.85f, 0.78f, 1.0f), ScreenWidth - 232.0f, ScreenHeight - 142.0f, nullptr, 0.7f);
    DrawText(TEXT("LMB / E FIRE REEL"), HudColor, ScreenWidth - 232.0f, ScreenHeight - 118.0f, nullptr, 0.68f);
    DrawText(TEXT("F / B BOARD-EXIT"), bBoardedBoat ? ValidColor : NeutralColor, ScreenWidth - 232.0f, ScreenHeight - 94.0f, nullptr, 0.68f);

    DrawPanel(28.0f, ScreenHeight - 208.0f, 390.0f, 162.0f, PanelColor);
    DrawText(bValidTarget ? TEXT("TARGET LOCK") : TEXT("TARGET SEARCH"), HudColor, 44.0f, ScreenHeight - 193.0f, nullptr, 0.82f);
    DrawText(FString::Printf(TEXT("DIST %.0fm"), TargetDistance / 100.0f), FLinearColor(0.86f, 0.86f, 0.78f, 1.0f), 282.0f, ScreenHeight - 193.0f, nullptr, 0.72f);
    DrawText(TEXT("LINE TENSION"), FLinearColor(0.85f, 0.85f, 0.78f, 1.0f), 44.0f, ScreenHeight - 166.0f, nullptr, 0.68f);
    DrawBar(144.0f, ScreenHeight - 160.0f, 174.0f, 8.0f, LineTension, HudColor);
    DrawText(LastReelResult, FLinearColor(0.75f, 1.0f, 0.78f, 1.0f), 44.0f, ScreenHeight - 138.0f, nullptr, 0.68f);
    DrawText(FString::Printf(TEXT("PULLS %d   RESCUES %d   BOATS %d"), GrapplePullCount, CivilianRescueCount, BoatTowCount), FLinearColor(0.84f, 0.92f, 1.0f, 1.0f), 44.0f, ScreenHeight - 116.0f, nullptr, 0.6f);
    DrawText(FString::Printf(TEXT("WATER %.0fcm   MOVE %.0f%%   %s"), WaterDepth, WaterMovementScale * 100.0f, bBoatableWater ? TEXT("BOATABLE") : TEXT("SHALLOW")), FLinearColor(0.48f, 0.82f, 1.0f, 1.0f), 44.0f, ScreenHeight - 94.0f, nullptr, 0.6f);
    DrawText(BoatStatus, bBoardedBoat ? ValidColor : NeutralColor, 44.0f, ScreenHeight - 72.0f, nullptr, 0.6f);
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

void ADSPrototypeHUD::DrawBar(float X, float Y, float Width, float Height, float Value, const FLinearColor& FillColor)
{
    const float ClampedValue = FMath::Clamp(Value, 0.0f, 1.0f);
    DrawRect(FLinearColor(0.02f, 0.03f, 0.03f, 0.72f), X, Y, Width, Height);
    DrawRect(FillColor, X, Y, Width * ClampedValue, Height);
    DrawLine(X, Y, X + Width, Y, FLinearColor(0.7f, 0.78f, 0.68f, 0.3f), 1.0f);
    DrawLine(X, Y + Height, X + Width, Y + Height, FLinearColor(0.7f, 0.78f, 0.68f, 0.24f), 1.0f);
}
