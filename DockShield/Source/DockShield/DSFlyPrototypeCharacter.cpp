#include "DSFlyPrototypeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "DSTargetableComponent.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"

ADSFlyPrototypeCharacter::ADSFlyPrototypeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->bOrientRotationToMovement = true;
    Movement->RotationRate = FRotator(0.0f, 420.0f, 0.0f);
    Movement->JumpZVelocity = 470.0f;
    Movement->AirControl = 0.42f;
    Movement->MaxWalkSpeed = 460.0f;
    Movement->MinAnalogWalkSpeed = 20.0f;
    Movement->BrakingDecelerationWalking = 1800.0f;
    Movement->BrakingDecelerationFalling = 1400.0f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 430.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingFinder(TEXT("/Game/Input/IMC_Default.IMC_Default"));
    if (MappingFinder.Succeeded())
    {
        DefaultMappingContext = MappingFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> MoveFinder(TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
    if (MoveFinder.Succeeded())
    {
        MoveAction = MoveFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> LookFinder(TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
    if (LookFinder.Succeeded())
    {
        LookAction = LookFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> JumpFinder(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
    if (JumpFinder.Succeeded())
    {
        JumpAction = JumpFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
    if (MeshFinder.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(MeshFinder.Object);
        GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -89.0f));
        GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimFinder(TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"));
    if (AnimFinder.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimFinder.Class);
    }
}

void ADSFlyPrototypeCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (DefaultMappingContext)
                {
                    Subsystem->AddMappingContext(DefaultMappingContext, 0);
                }
            }
        }
    }
}

void ADSFlyPrototypeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    CurrentReconTarget = FindBestReconTarget();
    DrawSonarDebug();

    if (AActor* Target = CurrentReconTarget.Get())
    {
        const FColor TargetColor = CanFlyMark(Target) ? FColor(0, 220, 160) : FColor::Orange;
        DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 60.0f, 16, TargetColor, false, 0.0f, 0, 3.0f);
        ShowDebugMessage(GetCurrentReconPrompt(), TargetColor);
    }
    else
    {
        ShowDebugMessage(TEXT("The Fly: Q sonar pulse, E/LMB mark target"), FColor::Cyan);
    }
}

void ADSFlyPrototypeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADSFlyPrototypeCharacter::Move);
        }
        if (LookAction)
        {
            EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADSFlyPrototypeCharacter::Look);
        }
        if (JumpAction)
        {
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        }
    }

    PlayerInputComponent->BindKey(EKeys::Q, IE_Pressed, this, &ADSFlyPrototypeCharacter::HandleSonarPulse);
    PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &ADSFlyPrototypeCharacter::TryFlyMark);
    PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ADSFlyPrototypeCharacter::TryFlyMark);
    PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &ADSFlyPrototypeCharacter::StartAim);
    PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Released, this, &ADSFlyPrototypeCharacter::StopAim);
}

int32 ADSFlyPrototypeCharacter::SonarPulse()
{
    LastSonarHitCount = 0;

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor || Actor == this || !CanFlyMark(Actor))
        {
            continue;
        }

        const float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
        if (Distance > SonarRange || !IsInsideSonarCone(Actor, -0.18f))
        {
            continue;
        }

        ++LastSonarHitCount;
        DrawDebugSphere(GetWorld(), Actor->GetActorLocation(), 76.0f, 20, FColor::Cyan, false, 1.2f, 0, 4.0f);
    }

    LastReconResult = FString::Printf(TEXT("SONAR PULSE %d"), LastSonarHitCount);
    ShowDebugMessage(LastReconResult, FColor::Cyan);
    return LastSonarHitCount;
}

bool ADSFlyPrototypeCharacter::ExecuteFlyMarkOnTarget(AActor* Target)
{
    UDSTargetableComponent* Targetable = GetTargetableComponent(Target);
    if (!Target || !Targetable)
    {
        LastReconResult = TEXT("NO RECON TARGET");
        ShowDebugMessage(TEXT("Fly Mark: no target"), FColor::Red);
        return false;
    }

    Targetable->ConfigureFromOwnerTags();
    if (!Targetable->CanFlyMark())
    {
        LastReconResult = TEXT("TARGET NOT MARKABLE");
        ShowDebugMessage(TEXT("Fly Mark: target cannot be marked"), FColor::Orange);
        return false;
    }

    const bool bWasMarked = Targetable->IsFlyMarked();
    const float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
    const float Strength = 1.0f - FMath::Clamp(Distance / FMath::Max(SonarRange, 1.0f), 0.0f, 0.72f);
    if (!Targetable->MarkForFly(Strength))
    {
        LastReconResult = TEXT("MARK FAILED");
        return false;
    }

    if (!bWasMarked)
    {
        ++MarkedTargetCount;
    }

    LastReconResult = FString::Printf(TEXT("FLY MARK %d"), MarkedTargetCount);
    DrawDebugLine(GetWorld(), FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation(), Target->GetActorLocation(), FColor::Cyan, false, 0.75f, 0, 5.0f);
    DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 92.0f, 24, FColor(0, 220, 160), false, 0.75f, 0, 4.0f);
    ShowDebugMessage(Targetable->GetFlyPrompt(), FColor(0, 220, 160));
    return true;
}

void ADSFlyPrototypeCharacter::TryFlyMark()
{
    ExecuteFlyMarkOnTarget(CurrentReconTarget.Get());
}

AActor* ADSFlyPrototypeCharacter::GetCurrentReconTargetActor() const
{
    return CurrentReconTarget.Get();
}

FString ADSFlyPrototypeCharacter::GetCurrentReconPrompt() const
{
    if (AActor* Target = CurrentReconTarget.Get())
    {
        if (UDSTargetableComponent* Targetable = GetTargetableComponent(Target))
        {
            return Targetable->GetFlyPrompt();
        }
        return FString::Printf(TEXT("Recon Target: %s"), *Target->GetName());
    }

    return TEXT("No recon target: pulse sonar or face a DockShield target");
}

bool ADSFlyPrototypeCharacter::IsCurrentReconTargetMarkValid() const
{
    return CanFlyMark(CurrentReconTarget.Get());
}

bool ADSFlyPrototypeCharacter::IsAiming() const
{
    return bIsAiming;
}

float ADSFlyPrototypeCharacter::GetSonarRange() const
{
    return SonarRange;
}

float ADSFlyPrototypeCharacter::GetSonarConeDegrees() const
{
    return SonarConeDegrees;
}

int32 ADSFlyPrototypeCharacter::GetLastSonarHitCount() const
{
    return LastSonarHitCount;
}

int32 ADSFlyPrototypeCharacter::GetMarkedTargetCount() const
{
    return MarkedTargetCount;
}

FString ADSFlyPrototypeCharacter::GetLastReconResult() const
{
    return LastReconResult;
}

void ADSFlyPrototypeCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    if (!Controller)
    {
        return;
    }

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MovementVector.Y);
    AddMovementInput(RightDirection, MovementVector.X);
}

void ADSFlyPrototypeCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void ADSFlyPrototypeCharacter::HandleSonarPulse()
{
    SonarPulse();
}

void ADSFlyPrototypeCharacter::StartAim()
{
    bIsAiming = true;
    bUseControllerRotationYaw = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = 285.0f;
    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = 350.0f;
    }
}

void ADSFlyPrototypeCharacter::StopAim()
{
    bIsAiming = false;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->MaxWalkSpeed = 460.0f;
    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = 430.0f;
    }
}

AActor* ADSFlyPrototypeCharacter::FindBestReconTarget() const
{
    AActor* BestTarget = nullptr;
    float BestScore = TNumericLimits<float>::Max();

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor || Actor == this || !CanFlyMark(Actor))
        {
            continue;
        }

        const float Distance = FVector::Dist(FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation(), Actor->GetActorLocation());
        if (Distance > SonarRange || !IsInsideSonarCone(Actor, bIsAiming ? 0.34f : 0.12f))
        {
            continue;
        }

        const FVector ViewForward = FollowCamera ? FollowCamera->GetForwardVector() : GetActorForwardVector();
        const FVector ToActor = (Actor->GetActorLocation() - (FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation())).GetSafeNormal();
        const float AimDot = FVector::DotProduct(ViewForward, ToActor);
        const float Score = Distance - (AimDot * 500.0f);
        if (Score < BestScore)
        {
            BestScore = Score;
            BestTarget = Actor;
        }
    }

    return BestTarget;
}

UDSTargetableComponent* ADSFlyPrototypeCharacter::GetTargetableComponent(AActor* Actor) const
{
    return Actor ? Actor->FindComponentByClass<UDSTargetableComponent>() : nullptr;
}

bool ADSFlyPrototypeCharacter::CanFlyMark(AActor* Actor) const
{
    if (UDSTargetableComponent* Targetable = GetTargetableComponent(Actor))
    {
        Targetable->ConfigureFromOwnerTags();
        return Targetable->CanFlyMark();
    }

    return false;
}

bool ADSFlyPrototypeCharacter::IsInsideSonarCone(AActor* Actor, float MinDot) const
{
    if (!Actor)
    {
        return false;
    }

    const FVector ViewLocation = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation();
    const FVector ViewForward = FollowCamera ? FollowCamera->GetForwardVector() : GetActorForwardVector();
    const FVector ToActor = (Actor->GetActorLocation() - ViewLocation).GetSafeNormal();
    return FVector::DotProduct(ViewForward, ToActor) >= MinDot;
}

void ADSFlyPrototypeCharacter::DrawSonarDebug() const
{
    if (!GetWorld())
    {
        return;
    }

    const FVector Start = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation() + FVector(0.0f, 0.0f, 64.0f);
    const FVector Forward = FollowCamera ? FollowCamera->GetForwardVector() : GetActorForwardVector();
    const FRotator ForwardRotation = Forward.Rotation();
    const FVector Left = (ForwardRotation + FRotator(0.0f, -SonarConeDegrees * 0.5f, 0.0f)).Vector();
    const FVector Right = (ForwardRotation + FRotator(0.0f, SonarConeDegrees * 0.5f, 0.0f)).Vector();
    const FColor SonarColor = bIsAiming ? FColor(0, 220, 160) : FColor::Cyan;

    DrawDebugLine(GetWorld(), Start, Start + (Forward * SonarRange), SonarColor, false, 0.0f, 0, 1.5f);
    DrawDebugLine(GetWorld(), Start, Start + (Left * SonarRange * 0.78f), SonarColor, false, 0.0f, 0, 1.0f);
    DrawDebugLine(GetWorld(), Start, Start + (Right * SonarRange * 0.78f), SonarColor, false, 0.0f, 0, 1.0f);
}

void ADSFlyPrototypeCharacter::ShowDebugMessage(const FString& Message, const FColor& Color) const
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(81058, 0.0f, Color, Message);
    }
}
