#include "DSPrototypePlayerController.h"
#include "DSFlyPrototypeCharacter.h"
#include "DSLillyPrototypeCharacter.h"
#include "DSReelPrototypeCharacter.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "InputCoreTypes.h"

ADSPrototypePlayerController::ADSPrototypePlayerController()
{
    ReelClass = ADSReelPrototypeCharacter::StaticClass();
    FlyClass = ADSFlyPrototypeCharacter::StaticClass();
    LillyClass = ADSLillyPrototypeCharacter::StaticClass();
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
    InputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ADSPrototypePlayerController::HandleSwitchToLillyInput);
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

bool ADSPrototypePlayerController::SwitchToLilly()
{
    return SwitchToRole(EDSPrototypeHeroRole::Lilly);
}

bool ADSPrototypePlayerController::CycleHero()
{
    switch (ActiveRole)
    {
    case EDSPrototypeHeroRole::Reel:
        return SwitchToRole(EDSPrototypeHeroRole::Fly);
    case EDSPrototypeHeroRole::Fly:
        return SwitchToRole(EDSPrototypeHeroRole::Lilly);
    default:
        return SwitchToRole(EDSPrototypeHeroRole::Reel);
    }
}

void ADSPrototypePlayerController::HandleSwitchToReelInput()
{
    SwitchToReel();
}

void ADSPrototypePlayerController::HandleSwitchToFlyInput()
{
    SwitchToFly();
}

void ADSPrototypePlayerController::HandleSwitchToLillyInput()
{
    SwitchToLilly();
}

void ADSPrototypePlayerController::HandleCycleHeroInput()
{
    CycleHero();
}

FString ADSPrototypePlayerController::GetActiveHeroLabel() const
{
    switch (ActiveRole)
    {
    case EDSPrototypeHeroRole::Reel:
        return TEXT("THE REEL");
    case EDSPrototypeHeroRole::Fly:
        return TEXT("THE FLY");
    default:
        return TEXT("LILLY LOCH");
    }
}

FString ADSPrototypePlayerController::GetSwitchStatusText() const
{
    return FString::Printf(TEXT("%s | 1 REEL  2 FLY  3 LILLY  TAB"), *LastSwitchResult);
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

AActor* ADSPrototypePlayerController::GetLillyPawnActor() const
{
    return LillyPawn.Get();
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
    else if (ADSLillyPrototypeCharacter* ControlledLilly = Cast<ADSLillyPrototypeCharacter>(GetPawn()))
    {
        LillyPawn = ControlledLilly;
        ActiveRole = EDSPrototypeHeroRole::Lilly;
        LastSwitchResult = TEXT("ACTIVE: LILLY LOCH");
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
    else if (TargetRole == EDSPrototypeHeroRole::Fly)
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
    else
    {
        TargetPawn = LillyPawn.Get();
        if (!TargetPawn && LillyClass)
        {
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.Owner = this;
            TargetPawn = World->SpawnActor<ADSLillyPrototypeCharacter>(LillyClass, GetSpawnLocationForRole(TargetRole), GetSpawnRotationForRole(), SpawnParameters);
            LillyPawn = Cast<ADSLillyPrototypeCharacter>(TargetPawn);
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

    float SideOffset = -160.0f;
    if (TargetRole == EDSPrototypeHeroRole::Fly)
    {
        SideOffset = 160.0f;
    }
    else if (TargetRole == EDSPrototypeHeroRole::Lilly)
    {
        SideOffset = -260.0f;
    }

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
