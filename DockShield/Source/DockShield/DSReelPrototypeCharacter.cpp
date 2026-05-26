#include "DSReelPrototypeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "DSTargetableComponent.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"

ADSReelPrototypeCharacter::ADSReelPrototypeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->bOrientRotationToMovement = true;
    Movement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    Movement->JumpZVelocity = 500.0f;
    Movement->AirControl = 0.35f;
    Movement->MaxWalkSpeed = 500.0f;
    Movement->MinAnalogWalkSpeed = 20.0f;
    Movement->BrakingDecelerationWalking = 2000.0f;
    Movement->BrakingDecelerationFalling = 1500.0f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 450.0f;
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

void ADSReelPrototypeCharacter::BeginPlay()
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

void ADSReelPrototypeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    CurrentTarget = FindBestTarget();
    if (AActor* Target = CurrentTarget.Get())
    {
        const FVector Start = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation();
        DrawDebugLine(GetWorld(), Start, Target->GetActorLocation(), FColor::Cyan, false, 0.0f, 0, 2.0f);
        DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 48.0f, 12, CanReelPull(Target) ? FColor::Green : FColor::Orange, false, 0.0f);

        if (UDSTargetableComponent* Targetable = GetTargetableComponent(Target))
        {
            ShowDebugMessage(Targetable->GetReelPrompt(), CanReelPull(Target) ? FColor::Green : FColor::Orange);
        }
        else
        {
            ShowDebugMessage(FString::Printf(TEXT("Target: %s"), *Target->GetActorLabel()), FColor::Cyan);
        }
    }
    else
    {
        ShowDebugMessage(TEXT("DockShield Reel v0: face a target and press E"), FColor::White);
    }
}

void ADSReelPrototypeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADSReelPrototypeCharacter::Move);
        }
        if (LookAction)
        {
            EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADSReelPrototypeCharacter::Look);
        }
        if (JumpAction)
        {
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        }
    }

    PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &ADSReelPrototypeCharacter::TryReelPull);
}

void ADSReelPrototypeCharacter::Move(const FInputActionValue& Value)
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

void ADSReelPrototypeCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void ADSReelPrototypeCharacter::TryReelPull()
{
    AActor* Target = CurrentTarget.Get();
    if (!Target || !CanReelPull(Target))
    {
        ShowDebugMessage(TEXT("Reel Pull: no valid target"), FColor::Red);
        return;
    }

    if (Target->ActorHasTag(TEXT("Civilian")))
    {
        const FVector RescueLocation = GetActorLocation() + (GetActorForwardVector() * 150.0f) + FVector(0.0f, 0.0f, 60.0f);
        Target->SetActorLocation(RescueLocation, false, nullptr, ETeleportType::TeleportPhysics);
        ShowDebugMessage(TEXT("Rescue Reel: civilian pulled clear"), FColor::Green);
        return;
    }

    const FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
    FVector LaunchVelocity = ToTarget.GetSafeNormal() * 900.0f;
    LaunchVelocity.Z = 320.0f;
    LaunchCharacter(LaunchVelocity, true, true);
    ShowDebugMessage(TEXT("Grapple Cast: pulling to target"), FColor::Green);
}

AActor* ADSReelPrototypeCharacter::FindBestTarget() const
{
    const FVector ViewLocation = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation();
    const FVector ViewForward = FollowCamera ? FollowCamera->GetForwardVector() : GetActorForwardVector();

    AActor* BestTarget = nullptr;
    float BestScore = TNumericLimits<float>::Max();

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor || Actor == this)
        {
            continue;
        }

        const bool bDockShieldTarget = Actor->ActorHasTag(TEXT("DockShieldTarget")) || Actor->FindComponentByClass<UDSTargetableComponent>();
        if (!bDockShieldTarget)
        {
            continue;
        }

        const FVector ToActor = Actor->GetActorLocation() - ViewLocation;
        const float Distance = ToActor.Size();
        if (Distance > 1200.0f)
        {
            continue;
        }

        const float AimDot = FVector::DotProduct(ViewForward, ToActor.GetSafeNormal());
        if (AimDot < 0.55f)
        {
            continue;
        }

        const float Score = Distance - (AimDot * 400.0f);
        if (Score < BestScore)
        {
            BestScore = Score;
            BestTarget = Actor;
        }
    }

    return BestTarget;
}

UDSTargetableComponent* ADSReelPrototypeCharacter::GetTargetableComponent(AActor* Actor) const
{
    return Actor ? Actor->FindComponentByClass<UDSTargetableComponent>() : nullptr;
}

bool ADSReelPrototypeCharacter::CanReelPull(AActor* Actor) const
{
    if (!Actor)
    {
        return false;
    }

    if (UDSTargetableComponent* Targetable = GetTargetableComponent(Actor))
    {
        Targetable->ConfigureFromOwnerTags();
        return Targetable->bCanReelPull;
    }

    return Actor->ActorHasTag(TEXT("GrapplePoint")) || Actor->ActorHasTag(TEXT("Civilian"));
}

void ADSReelPrototypeCharacter::ShowDebugMessage(const FString& Message, const FColor& Color) const
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(81057, 0.0f, Color, Message);
    }
}
