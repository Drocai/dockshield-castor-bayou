#include "DSPrototypeHUD.h"
#include "DSFlyPrototypeCharacter.h"
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
    const ADSFlyPrototypeCharacter* FlyCharacter = nullptr;
    if (APlayerController* PlayerController = GetOwningPlayerController())
    {
        APawn* ControlledPawn = PlayerController->GetPawn();
        ReelCharacter = Cast<ADSReelPrototypeCharacter>(ControlledPawn);
        FlyCharacter = Cast<ADSFlyPrototypeCharacter>(ControlledPawn);
    }

    if (FlyCharacter)
    {
        DrawFlyHUD(FlyCharacter, ScreenWidth, ScreenHeight, CenterX, CenterY);
        return;
    }

    const bool bLineAttached = ReelCharacter && ReelCharacter->IsReelLineAttached();
    const bool bReelHoldActive = ReelCharacter && ReelCharacter->IsReelHoldActive();
    const bool bHasTarget = ReelCharacter && (ReelCharacter->GetCurrentTargetActor() || bLineAttached);
    const bool bValidTarget = ReelCharacter && ReelCharacter->IsCurrentTargetReelPullValid();
    const bool bAiming = ReelCharacter && ReelCharacter->IsAiming();
    const float TargetDistance = ReelCharacter ? ReelCharacter->GetCurrentTargetDistance() : 0.0f;
    const float LineTension = ReelCharacter ? ReelCharacter->GetLineTension() : 0.0f;
    const int32 ReelSnapCount = ReelCharacter ? ReelCharacter->GetReelSnapCount() : 0;
    const int32 GrapplePullCount = ReelCharacter ? ReelCharacter->GetGrapplePullCount() : 0;
    const int32 CivilianRescueCount = ReelCharacter ? ReelCharacter->GetCivilianRescueCount() : 0;
    const int32 BoatTowCount = ReelCharacter ? ReelCharacter->GetBoatTowCount() : 0;
    const float WaterDepth = ReelCharacter ? ReelCharacter->GetCurrentWaterDepth() : 0.0f;
    const float WaterMovementScale = ReelCharacter ? ReelCharacter->GetWaterMovementScale() : 1.0f;
    const float WaterPressure = ReelCharacter ? ReelCharacter->GetCurrentWaterPressure() : 0.0f;
    const float WaterCurrentSpeed = ReelCharacter ? ReelCharacter->GetCurrentWaterSpeed() : 0.0f;
    const float ObjectiveProgress = ReelCharacter ? ReelCharacter->GetPrototypeObjectiveProgress() : 0.0f;
    const bool bBoatableWater = ReelCharacter && ReelCharacter->IsInBoatableWater();
    const bool bBoardedBoat = ReelCharacter && ReelCharacter->IsBoardedBoat();
    FString Prompt = TEXT("DockShield Reel v0: acquire target");
    FString LastReelResult = TEXT("READY");
    FString BoatStatus = TEXT("NO BOAT TARGET");
    FString ReelLineState = TEXT("IDLE");
    FString ObjectiveText = TEXT("OBJECTIVE: Test Cast / Rescue / Tow");
    FString WaterState = TEXT("DRY");
    if (ReelCharacter)
    {
        Prompt = ReelCharacter->GetCurrentTargetPrompt();
        LastReelResult = ReelCharacter->GetLastReelResult();
        BoatStatus = ReelCharacter->GetBoatStatusText();
        ReelLineState = ReelCharacter->GetReelLineStateText();
        ObjectiveText = ReelCharacter->GetPrototypeObjectiveText();
        WaterState = ReelCharacter->GetWaterStatusText();
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
    if (ReelLineState.Len() > 16)
    {
        ReelLineState = ReelLineState.Left(13) + TEXT("...");
    }
    if (ObjectiveText.Len() > 42)
    {
        ObjectiveText = ObjectiveText.Left(39) + TEXT("...");
    }

    const FLinearColor ValidColor(0.05f, 0.95f, 0.35f, 1.0f);
    const FLinearColor InvalidColor(1.0f, 0.55f, 0.08f, 1.0f);
    const FLinearColor NeutralColor(0.84f, 0.92f, 1.0f, 1.0f);
    const FLinearColor HudColor = bValidTarget ? ValidColor : (bHasTarget ? InvalidColor : NeutralColor);
    FLinearColor LineStateColor = HudColor;
    if (ReelLineState == TEXT("SNAPPED") || ReelLineState == TEXT("DANGER"))
    {
        LineStateColor = FLinearColor(1.0f, 0.14f, 0.08f, 1.0f);
    }
    else if (ReelLineState == TEXT("STRAIN"))
    {
        LineStateColor = FLinearColor(1.0f, 0.78f, 0.12f, 1.0f);
    }
    else if (bReelHoldActive || ReelLineState == TEXT("REELING"))
    {
        LineStateColor = ValidColor;
    }
    else if (bLineAttached)
    {
        LineStateColor = FLinearColor(0.16f, 0.8f, 1.0f, 1.0f);
    }
    const FLinearColor WaterColor = WaterPressure >= 0.65f
        ? FLinearColor(1.0f, 0.18f, 0.08f, 1.0f)
        : (WaterPressure >= 0.35f ? FLinearColor(0.9f, 0.72f, 0.18f, 1.0f) : FLinearColor(0.48f, 0.82f, 1.0f, 1.0f));
    const FLinearColor PanelColor(0.0f, 0.0f, 0.0f, 0.48f);

    DrawReticle(CenterX, CenterY, bLineAttached ? LineStateColor : HudColor);

    DrawPanel(28.0f, 28.0f, 300.0f, 118.0f, PanelColor);
    DrawText(TEXT("THE REEL"), FLinearColor(0.85f, 0.95f, 1.0f, 1.0f), 44.0f, 42.0f, nullptr, 1.45f);
    DrawText(TEXT("PUBLIC HERO"), FLinearColor(0.2f, 0.7f, 1.0f, 1.0f), 46.0f, 74.0f, nullptr, 0.82f);
    DrawText(bBoardedBoat ? TEXT("BOAT CONTROL") : (bAiming ? TEXT("AIM MODE") : TEXT("MOVE MODE")), bBoardedBoat ? ValidColor : (bAiming ? ValidColor : NeutralColor), 188.0f, 74.0f, nullptr, 0.72f);
    DrawText(ObjectiveText, FLinearColor(0.75f, 1.0f, 0.78f, 1.0f), 44.0f, 106.0f, nullptr, 0.72f);

    DrawPanel(ScreenWidth - 336.0f, 28.0f, 308.0f, 96.0f, PanelColor);
    DrawText(TEXT("M_TEST_TARGETING"), FLinearColor(0.86f, 0.86f, 0.78f, 1.0f), ScreenWidth - 318.0f, 44.0f, nullptr, 0.82f);
    DrawText(TEXT("TARGETING RANGE"), FLinearColor(1.0f, 0.42f, 0.35f, 1.0f), ScreenWidth - 318.0f, 72.0f, nullptr, 0.75f);
    DrawBar(ScreenWidth - 318.0f, 102.0f, 184.0f, 6.0f, ObjectiveProgress, ValidColor);

    const float PromptWidth = 560.0f;
    const float PromptHeight = 56.0f;
    DrawPanel(CenterX - (PromptWidth * 0.5f), ScreenHeight - 132.0f, PromptWidth, PromptHeight, PanelColor);
    DrawText(Prompt, bLineAttached ? LineStateColor : HudColor, CenterX - (PromptWidth * 0.5f) + 18.0f, ScreenHeight - 115.0f, nullptr, 0.86f);

    DrawPanel(ScreenWidth - 250.0f, ScreenHeight - 178.0f, 222.0f, 110.0f, PanelColor);
    DrawText(TEXT("RMB AIM"), FLinearColor(0.85f, 0.85f, 0.78f, 1.0f), ScreenWidth - 232.0f, ScreenHeight - 166.0f, nullptr, 0.7f);
    DrawText(TEXT("LMB / E CAST LINE"), bLineAttached ? LineStateColor : HudColor, ScreenWidth - 232.0f, ScreenHeight - 142.0f, nullptr, 0.68f);
    DrawText(TEXT("R HOLD REEL"), bReelHoldActive ? ValidColor : LineStateColor, ScreenWidth - 232.0f, ScreenHeight - 118.0f, nullptr, 0.68f);
    DrawText(TEXT("F / B BOARD-EXIT"), bBoardedBoat ? ValidColor : NeutralColor, ScreenWidth - 232.0f, ScreenHeight - 94.0f, nullptr, 0.68f);

    DrawPanel(28.0f, ScreenHeight - 230.0f, 390.0f, 184.0f, PanelColor);
    DrawText(bLineAttached ? TEXT("LINE ATTACHED") : (bValidTarget ? TEXT("TARGET LOCK") : TEXT("TARGET SEARCH")), bLineAttached ? LineStateColor : HudColor, 44.0f, ScreenHeight - 215.0f, nullptr, 0.82f);
    DrawText(FString::Printf(TEXT("DIST %.0fm"), TargetDistance / 100.0f), FLinearColor(0.86f, 0.86f, 0.78f, 1.0f), 282.0f, ScreenHeight - 215.0f, nullptr, 0.72f);
    DrawText(TEXT("LINE TENSION"), FLinearColor(0.85f, 0.85f, 0.78f, 1.0f), 44.0f, ScreenHeight - 188.0f, nullptr, 0.68f);
    DrawBar(144.0f, ScreenHeight - 182.0f, 174.0f, 8.0f, LineTension, LineStateColor);
    DrawText(FString::Printf(TEXT("LINE %s   SNAPS %d"), *ReelLineState, ReelSnapCount), LineStateColor, 44.0f, ScreenHeight - 160.0f, nullptr, 0.62f);
    DrawText(LastReelResult, FLinearColor(0.75f, 1.0f, 0.78f, 1.0f), 44.0f, ScreenHeight - 138.0f, nullptr, 0.68f);
    DrawText(FString::Printf(TEXT("PULLS %d   RESCUES %d   BOATS %d"), GrapplePullCount, CivilianRescueCount, BoatTowCount), FLinearColor(0.84f, 0.92f, 1.0f, 1.0f), 44.0f, ScreenHeight - 116.0f, nullptr, 0.6f);
    DrawText(FString::Printf(TEXT("%s %.0fcm   MOVE %.0f%%   %s"), *WaterState, WaterDepth, WaterMovementScale * 100.0f, bBoatableWater ? TEXT("BOATABLE") : TEXT("SHALLOW")), WaterColor, 44.0f, ScreenHeight - 94.0f, nullptr, 0.6f);
    DrawText(FString::Printf(TEXT("PRESSURE %.0f%%   CURRENT %.0fcm/s"), WaterPressure * 100.0f, WaterCurrentSpeed), WaterColor, 44.0f, ScreenHeight - 83.0f, nullptr, 0.52f);
    DrawText(BoatStatus, bBoardedBoat ? ValidColor : NeutralColor, 44.0f, ScreenHeight - 72.0f, nullptr, 0.6f);
}

void ADSPrototypeHUD::DrawFlyHUD(const ADSFlyPrototypeCharacter* FlyCharacter, float ScreenWidth, float ScreenHeight, float CenterX, float CenterY)
{
    const bool bHasTarget = FlyCharacter && FlyCharacter->GetCurrentReconTargetActor();
    const bool bValidTarget = FlyCharacter && FlyCharacter->IsCurrentReconTargetMarkValid();
    const bool bAiming = FlyCharacter && FlyCharacter->IsAiming();
    const int32 SonarHits = FlyCharacter ? FlyCharacter->GetLastSonarHitCount() : 0;
    const int32 MarkedTargets = FlyCharacter ? FlyCharacter->GetMarkedTargetCount() : 0;
    const float SonarRange = FlyCharacter ? FlyCharacter->GetSonarRange() : 0.0f;
    FString Prompt = FlyCharacter ? FlyCharacter->GetCurrentReconPrompt() : TEXT("No Fly pawn");
    FString LastReconResult = FlyCharacter ? FlyCharacter->GetLastReconResult() : TEXT("NO RECON");

    if (Prompt.Len() > 72)
    {
        Prompt = Prompt.Left(69) + TEXT("...");
    }
    if (LastReconResult.Len() > 34)
    {
        LastReconResult = LastReconResult.Left(31) + TEXT("...");
    }

    const FLinearColor ValidColor(0.0f, 0.86f, 0.62f, 1.0f);
    const FLinearColor InvalidColor(1.0f, 0.55f, 0.08f, 1.0f);
    const FLinearColor NeutralColor(0.70f, 0.95f, 1.0f, 1.0f);
    const FLinearColor HudColor = bValidTarget ? ValidColor : (bHasTarget ? InvalidColor : NeutralColor);
    const FLinearColor PanelColor(0.0f, 0.0f, 0.0f, 0.50f);

    DrawReticle(CenterX, CenterY, HudColor);

    DrawPanel(28.0f, 28.0f, 312.0f, 150.0f, PanelColor);
    DrawText(TEXT("THE FLY"), FLinearColor(0.72f, 1.0f, 0.88f, 1.0f), 44.0f, 42.0f, nullptr, 1.42f);
    DrawText(TEXT("COVERT HUNTER"), FLinearColor(0.0f, 0.86f, 0.62f, 1.0f), 46.0f, 74.0f, nullptr, 0.80f);
    DrawText(bAiming ? TEXT("SONAR AIM") : TEXT("STEALTH MOVE"), bAiming ? ValidColor : NeutralColor, 188.0f, 74.0f, nullptr, 0.72f);
    DrawText(TEXT("OBJECTIVE: Mark Targets"), FLinearColor(0.75f, 1.0f, 0.78f, 1.0f), 44.0f, 106.0f, nullptr, 0.72f);
    DrawText(FString::Printf(TEXT("SONAR %.0fm   MARKS %d"), SonarRange / 100.0f, MarkedTargets), NeutralColor, 44.0f, 132.0f, nullptr, 0.62f);

    DrawPanel(ScreenWidth - 336.0f, 28.0f, 308.0f, 96.0f, PanelColor);
    DrawText(TEXT("M_TEST_TARGETING"), FLinearColor(0.86f, 0.86f, 0.78f, 1.0f), ScreenWidth - 318.0f, 44.0f, nullptr, 0.82f);
    DrawText(TEXT("RECON RANGE"), FLinearColor(0.0f, 0.86f, 0.62f, 1.0f), ScreenWidth - 318.0f, 72.0f, nullptr, 0.75f);
    DrawBar(ScreenWidth - 318.0f, 102.0f, 184.0f, 6.0f, FMath::Clamp(SonarHits / 5.0f, 0.0f, 1.0f), HudColor);

    const float PromptWidth = 560.0f;
    const float PromptHeight = 56.0f;
    DrawPanel(CenterX - (PromptWidth * 0.5f), ScreenHeight - 132.0f, PromptWidth, PromptHeight, PanelColor);
    DrawText(Prompt, HudColor, CenterX - (PromptWidth * 0.5f) + 18.0f, ScreenHeight - 115.0f, nullptr, 0.86f);

    DrawPanel(ScreenWidth - 250.0f, ScreenHeight - 160.0f, 222.0f, 92.0f, PanelColor);
    DrawText(TEXT("Q SONAR PULSE"), NeutralColor, ScreenWidth - 232.0f, ScreenHeight - 148.0f, nullptr, 0.70f);
    DrawText(TEXT("LMB / E MARK"), HudColor, ScreenWidth - 232.0f, ScreenHeight - 124.0f, nullptr, 0.70f);
    DrawText(TEXT("RMB AIM"), bAiming ? ValidColor : NeutralColor, ScreenWidth - 232.0f, ScreenHeight - 100.0f, nullptr, 0.70f);

    DrawPanel(28.0f, ScreenHeight - 186.0f, 364.0f, 140.0f, PanelColor);
    DrawText(TEXT("FLY SENSE"), HudColor, 44.0f, ScreenHeight - 170.0f, nullptr, 0.82f);
    DrawText(FString::Printf(TEXT("SONAR HITS %d"), SonarHits), NeutralColor, 44.0f, ScreenHeight - 140.0f, nullptr, 0.68f);
    DrawText(FString::Printf(TEXT("MARKED TARGETS %d"), MarkedTargets), ValidColor, 44.0f, ScreenHeight - 116.0f, nullptr, 0.68f);
    DrawText(LastReconResult, FLinearColor(0.75f, 1.0f, 0.78f, 1.0f), 44.0f, ScreenHeight - 92.0f, nullptr, 0.68f);
    DrawText(TEXT("SQUAD STACK: REEL / FLY / LILLY"), FLinearColor(0.84f, 0.92f, 1.0f, 1.0f), 44.0f, ScreenHeight - 68.0f, nullptr, 0.56f);
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
