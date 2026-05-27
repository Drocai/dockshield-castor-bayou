#include "DSPrototypePlayerController.h"
#include "DSFlyPrototypeCharacter.h"
#include "DSReelPrototypeCharacter.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "InputCoreTypes.h"

ADSPrototypePlayerController::ADSPrototypePlayerController()
{
    ReelClass = ADSReelPrototypeCharacter::StaticClass();
    FlyClass = ADSFlyPrototypeCharacter::StaticClass();
}

void ADSPrototypePlayerController::BeginPlay()
{
    Super::BeginPlay();
    RegisterCurrentPawn();
    ShowSwitchMessage();
}

void ADSPrototypePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent)
    {
        return;
    }

    InputComponent->BindKey(EKeys::One, IE_Pressed, this, &ADSPrototypePlayerController::HandleSwitchToReelInput);
    InputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ADSPrototypePlayerController::HandleSwitchToFlyInput);
    InputComponent->BindKey(EKeys::Tab, IE_Pressed, this, &ADSPrototypePlayerController::HandleCycleHeroInput);
}

bool ADSPrototypePlayerController::SwitchToReel()
{
    return SwitchToRole(EDSPrototypeHeroRole::Reel);
}

bool ADSPrototypePlayerController::SwitchToFly()
{
    return SwitchToRole(EDSPrototypeHeroRole::Fly);
}

bool ADSPrototypePlayerController::CycleHero()
{
    return SwitchToRole(ActiveRole == EDSPrototypeHeroRole::Reel ? EDSPrototypeHeroRole::Fly : EDSPrototypeHeroRole::Reel);
}

void ADSPrototypePlayerController::HandleSwitchToReelInput()
{
    SwitchToReel();
}

void ADSPrototypePlayerController::HandleSwitchToFlyInput()
{
    SwitchToFly();
}

void ADSPrototypePlayerController::HandleCycleHeroInput()
{
    CycleHero();
}

FString ADSPrototypePlayerController::GetActiveHeroLabel() const
{
    return ActiveRole == EDSPrototypeHeroRole::Reel ? TEXT("THE REEL") : TEXT("THE FLY");
}

FString ADSPrototypePlayerController::GetSwitchStatusText() const
{
    return FString::Printf(TEXT("%s | 1 REEL  2 FLY  TAB CYCLE"), *LastSwitchResult);
}

int32 ADSPrototypePlayerController::GetSwitchCount() const
{
    return SwitchCount;
}

AActor* ADSPrototypePlayerController::GetReelPawnActor() const
{
    return ReelPawn.Get();
}

AActor* ADSPrototypePlayerController::GetFlyPawnActor() const
{
    return FlyPawn.Get();
}

void ADSPrototypePlayerController::RegisterCurrentPawn()
{
    if (ADSReelPrototypeCharacter* ControlledReel = Cast<ADSReelPrototypeCharacter>(GetPawn()))
    {
        ReelPawn = ControlledReel;
        ActiveRole = EDSPrototypeHeroRole::Reel;
        LastSwitchResult = TEXT("ACTIVE: THE REEL");
    }
    else if (ADSFlyPrototypeCharacter* ControlledFly = Cast<ADSFlyPrototypeCharacter>(GetPawn()))
    {
        FlyPawn = ControlledFly;
        ActiveRole = EDSPrototypeHeroRole::Fly;
        LastSwitchResult = TEXT("ACTIVE: THE FLY");
    }
}

bool ADSPrototypePlayerController::SwitchToRole(EDSPrototypeHeroRole TargetRole)
{
    RegisterCurrentPawn();

    UWorld* World = GetWorld();
    if (!World)
    {
        LastSwitchResult = TEXT("SWITCH FAILED: NO WORLD");
        return false;
    }

    APawn* TargetPawn = nullptr;
    if (TargetRole == EDSPrototypeHeroRole::Reel)
    {
        TargetPawn = ReelPawn.Get();
        if (!TargetPawn && ReelClass)
        {
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.Owner = this;
            TargetPawn = World->SpawnActor<ADSReelPrototypeCharacter>(ReelClass, GetSpawnLocationForRole(TargetRole), GetSpawnRotationForRole(), SpawnParameters);
            ReelPawn = Cast<ADSReelPrototypeCharacter>(TargetPawn);
        }
    }
    else
    {
        TargetPawn = FlyPawn.Get();
        if (!TargetPawn && FlyClass)
        {
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.Owner = this;
            TargetPawn = World->SpawnActor<ADSFlyPrototypeCharacter>(FlyClass, GetSpawnLocationForRole(TargetRole), GetSpawnRotationForRole(), SpawnParameters);
            FlyPawn = Cast<ADSFlyPrototypeCharacter>(TargetPawn);
        }
    }

    if (!TargetPawn)
    {
        LastSwitchResult = TEXT("SWITCH FAILED: MISSING HERO");
        ShowSwitchMessage();
        return false;
    }

    if (GetPawn() == TargetPawn)
    {
        ActiveRole = TargetRole;
        LastSwitchResult = FString::Printf(TEXT("ACTIVE: %s"), *GetActiveHeroLabel());
        ShowSwitchMessage();
        return true;
    }

    Possess(TargetPawn);
    ActiveRole = TargetRole;
    ++SwitchCount;
    LastSwitchResult = FString::Printf(TEXT("SWITCHED: %s"), *GetActiveHeroLabel());
    ShowSwitchMessage();
    return true;
}

FVector ADSPrototypePlayerController::GetSpawnLocationForRole(EDSPrototypeHeroRole TargetRole) const
{
    const APawn* CurrentPawn = GetPawn();
    if (!CurrentPawn)
    {
        return FVector::ZeroVector;
    }

    const float SideOffset = TargetRole == EDSPrototypeHeroRole::Fly ? 160.0f : -160.0f;
    return CurrentPawn->GetActorLocation() + (CurrentPawn->GetActorRightVector() * SideOffset);
}

FRotator ADSPrototypePlayerController::GetSpawnRotationForRole() const
{
    if (const APawn* CurrentPawn = GetPawn())
    {
        return CurrentPawn->GetActorRotation();
    }

    return FRotator::ZeroRotator;
}

void ADSPrototypePlayerController::ShowSwitchMessage() const
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(82031, 2.0f, FColor::Cyan, GetSwitchStatusText());
    }
}
