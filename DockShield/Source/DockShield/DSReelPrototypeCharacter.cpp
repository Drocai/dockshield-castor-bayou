#include "DSReelPrototypeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "DSPrototypeBoatActor.h"
#include "DSTargetableComponent.h"
#include "DSWaterZoneActor.h"
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

    UpdateWaterState(DeltaSeconds);
    CurrentTarget = FindBestTarget();
    if (AActor* Target = CurrentTarget.Get())
    {
        UpdateTargetMetrics(Target);
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
        UpdateTargetMetrics(nullptr);
        ShowDebugMessage(TEXT("DockShield Reel v0: face a target and press LMB/E to cast, hold R to reel"), FColor::White);
    }

    UpdateReelLine(DeltaSeconds);
    DrawReelFeedback(DeltaSeconds);
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

    PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &ADSReelPrototypeCharacter::TryCastReelLine);
    PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ADSReelPrototypeCharacter::TryCastReelLine);
    PlayerInputComponent->BindKey(EKeys::R, IE_Pressed, this, &ADSReelPrototypeCharacter::StartReelHold);
    PlayerInputComponent->BindKey(EKeys::R, IE_Released, this, &ADSReelPrototypeCharacter::StopReelHold);
    PlayerInputComponent->BindKey(EKeys::F, IE_Pressed, this, &ADSReelPrototypeCharacter::HandleBoardOrExitInput);
    PlayerInputComponent->BindKey(EKeys::B, IE_Pressed, this, &ADSReelPrototypeCharacter::HandleBoardOrExitInput);
    PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &ADSReelPrototypeCharacter::StartAim);
    PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Released, this, &ADSReelPrototypeCharacter::StopAim);
}

AActor* ADSReelPrototypeCharacter::GetCurrentTargetActor() const
{
    return CurrentTarget.Get();
}

FString ADSReelPrototypeCharacter::GetCurrentTargetPrompt() const
{
    if (bIsBoardedBoat)
    {
        return TEXT("F/B: Exit boat | WASD: pilot rescue boat | LMB/E: cast | hold R: reel");
    }

    if (AActor* AttachedTarget = AttachedReelTarget.Get())
    {
        return FString::Printf(TEXT("LINE %s -> %s | Hold R to reel | Release R to ease"), *ReelLineStateText, *AttachedTarget->GetName());
    }

    AActor* Target = CurrentTarget.Get();
    if (!Target)
    {
        return TEXT("Face a DockShield target and press LMB/E to cast");
    }

    if (const ADSPrototypeBoatActor* Boat = Cast<ADSPrototypeBoatActor>(Target))
    {
        const bool bCloseEnough = FVector::Dist(GetActorLocation(), Boat->GetActorLocation()) <= BoardInteractionRange;
        const FString BoardPrompt = bCloseEnough ? TEXT("F/B: Board boat") : TEXT("Move closer to board");
        return FString::Printf(TEXT("%s | Boat %s | LMB/E: Cast tow line | R: Reel"), *BoardPrompt, *Boat->GetBoatStateText());
    }

    if (UDSTargetableComponent* Targetable = GetTargetableComponent(Target))
    {
        return Targetable->GetReelPrompt();
    }

    return FString::Printf(TEXT("Target: %s"), *Target->GetName());
}

bool ADSReelPrototypeCharacter::IsCurrentTargetReelPullValid() const
{
    return CanReelPull(CurrentTarget.Get());
}

bool ADSReelPrototypeCharacter::IsAiming() const
{
    return bIsAiming;
}

float ADSReelPrototypeCharacter::GetCurrentTargetDistance() const
{
    return CurrentTargetDistance;
}

float ADSReelPrototypeCharacter::GetLineTension() const
{
    return FMath::Clamp(LineTension, 0.0f, 1.0f);
}

FString ADSReelPrototypeCharacter::GetLastReelResult() const
{
    return LastReelResult;
}

bool ADSReelPrototypeCharacter::IsReelLineAttached() const
{
    return AttachedReelTarget.IsValid();
}

bool ADSReelPrototypeCharacter::IsReelHoldActive() const
{
    return bReelHoldActive;
}

FString ADSReelPrototypeCharacter::GetReelLineStateText() const
{
    return ReelLineStateText;
}

AActor* ADSReelPrototypeCharacter::GetAttachedReelTarget() const
{
    return AttachedReelTarget.Get();
}

int32 ADSReelPrototypeCharacter::GetReelSnapCount() const
{
    return ReelSnapCount;
}

int32 ADSReelPrototypeCharacter::GetGrapplePullCount() const
{
    return GrapplePullCount;
}

int32 ADSReelPrototypeCharacter::GetCivilianRescueCount() const
{
    return CivilianRescueCount;
}

int32 ADSReelPrototypeCharacter::GetBoatTowCount() const
{
    return BoatTowCount;
}

float ADSReelPrototypeCharacter::GetCurrentWaterDepth() const
{
    return CurrentWaterDepth;
}

float ADSReelPrototypeCharacter::GetWaterMovementScale() const
{
    return WaterMovementScale;
}

bool ADSReelPrototypeCharacter::IsInBoatableWater() const
{
    return bInBoatableWater;
}

float ADSReelPrototypeCharacter::GetCurrentWaterPressure() const
{
    return CurrentWaterPressure;
}

FVector ADSReelPrototypeCharacter::GetCurrentWaterVelocity() const
{
    return CurrentWaterVelocity;
}

float ADSReelPrototypeCharacter::GetCurrentWaterSpeed() const
{
    return CurrentWaterVelocity.Size();
}

FString ADSReelPrototypeCharacter::GetWaterStatusText() const
{
    return CurrentWaterStateText;
}

FString ADSReelPrototypeCharacter::GetPrototypeObjectiveText() const
{
    if (CivilianRescueCount <= 0)
    {
        return TEXT("OBJECTIVE: Rescue civilian");
    }

    if (BoatTowCount <= 0)
    {
        return TEXT("OBJECTIVE: Tow rescue boat");
    }

    if (CurrentWaterPressure >= 0.65f)
    {
        return TEXT("OBJECTIVE: Escape flood surge");
    }

    return TEXT("OBJECTIVE: Extraction secured");
}

float ADSReelPrototypeCharacter::GetPrototypeObjectiveProgress() const
{
    float Progress = 0.0f;
    Progress += CivilianRescueCount > 0 ? 0.42f : 0.0f;
    Progress += BoatTowCount > 0 ? 0.38f : 0.0f;
    Progress += CurrentWaterPressure < 0.65f ? 0.20f : 0.08f;
    return FMath::Clamp(Progress, 0.0f, 1.0f);
}

bool ADSReelPrototypeCharacter::IsBoardedBoat() const
{
    return bIsBoardedBoat && BoardedBoat.IsValid();
}

FString ADSReelPrototypeCharacter::GetBoatStatusText() const
{
    if (const ADSPrototypeBoatActor* Boat = BoardedBoat.Get())
    {
        return FString::Printf(TEXT("BOAT %s | PILOT %.0fcm"), *Boat->GetBoatStateText(), Boat->GetLastPilotDistance());
    }

    if (const ADSPrototypeBoatActor* Boat = Cast<ADSPrototypeBoatActor>(CurrentTarget.Get()))
    {
        return FString::Printf(TEXT("BOAT %s | F/B BOARD"), *Boat->GetBoatStateText());
    }

    return bInBoatableWater ? TEXT("BOATABLE WATER") : TEXT("NO BOAT TARGET");
}

void ADSReelPrototypeCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    if (!Controller)
    {
        return;
    }

    if (ADSPrototypeBoatActor* Boat = BoardedBoat.Get())
    {
        const float DeltaSeconds = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.0f;
        if (Boat->ApplyPilotInput(MovementVector, Controller->GetControlRotation().Yaw, DeltaSeconds))
        {
            LastReelResult = TEXT("BOAT PILOT");
        }
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

void ADSReelPrototypeCharacter::StartAim()
{
    bIsAiming = true;
    bUseControllerRotationYaw = true;

    if (bIsBoardedBoat)
    {
        if (CameraBoom)
        {
            CameraBoom->TargetArmLength = 430.0f;
        }

        ShowDebugMessage(TEXT("Boat aim: LMB/E casts tow line, hold R to reel"), FColor::Cyan);
        return;
    }

    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->bOrientRotationToMovement = false;
    ApplyMovementSpeed();

    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = 360.0f;
    }

    ShowDebugMessage(TEXT("Aim locked: LMB/E casts line, hold R to reel"), FColor::Cyan);
}

void ADSReelPrototypeCharacter::StopAim()
{
    bIsAiming = false;
    bUseControllerRotationYaw = false;

    if (bIsBoardedBoat)
    {
        if (CameraBoom)
        {
            CameraBoom->TargetArmLength = 520.0f;
        }
        return;
    }

    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->bOrientRotationToMovement = true;
    ApplyMovementSpeed();

    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = 450.0f;
    }
}

void ADSReelPrototypeCharacter::TryReelPull()
{
    ExecuteReelActionOnTarget(CurrentTarget.Get());
}

void ADSReelPrototypeCharacter::TryCastReelLine()
{
    if (AttachedReelTarget.IsValid())
    {
        DetachReelLineInternal(TEXT("LINE DETACHED"), false);
        return;
    }

    CastReelLineAtTarget(CurrentTarget.Get());
}

bool ADSReelPrototypeCharacter::CastReelLineAtTarget(AActor* Target)
{
    if (!Target || !CanReelPull(Target))
    {
        StartReelFeedback(Target, FColor::Red);
        LastReelResult = TEXT("NO VALID TARGET");
        ReelLineStateText = TEXT("IDLE");
        ShowDebugMessage(TEXT("Cast Reel: no valid target"), FColor::Red);
        return false;
    }

    AttachedReelTarget = Target;
    bReelHoldActive = false;
    UpdateTargetMetrics(Target);
    RawLineTension = FMath::Clamp(LineTension * 0.72f, 0.08f, 0.72f);
    LineTension = RawLineTension;
    RefreshReelLineState();

    StartReelFeedback(Target, FColor::Cyan);
    LastReelResult = TEXT("LINE CAST");
    ShowDebugMessage(TEXT("Reel Line cast: hold R to reel, release to ease tension"), FColor::Cyan);
    return true;
}

void ADSReelPrototypeCharacter::StartReelHold()
{
    if (!AttachedReelTarget.IsValid())
    {
        if (!CastReelLineAtTarget(CurrentTarget.Get()))
        {
            return;
        }
    }

    bReelHoldActive = true;
    RefreshReelLineState();
    LastReelResult = TEXT("REELING");
}

void ADSReelPrototypeCharacter::StopReelHold()
{
    bReelHoldActive = false;
    RefreshReelLineState();
}

void ADSReelPrototypeCharacter::DetachReelLine()
{
    DetachReelLineInternal(TEXT("LINE DETACHED"), false);
}

void ADSReelPrototypeCharacter::HandleBoardOrExitInput()
{
    TryBoardOrExitBoat();
}

bool ADSReelPrototypeCharacter::TryBoardOrExitBoat()
{
    if (IsBoardedBoat())
    {
        ExitBoardedBoat();
        return true;
    }

    bIsBoardedBoat = false;
    BoardedBoat.Reset();

    ADSPrototypeBoatActor* Boat = Cast<ADSPrototypeBoatActor>(CurrentTarget.Get());
    if (!Boat)
    {
        LastReelResult = TEXT("NO BOAT TARGET");
        ShowDebugMessage(TEXT("Board: face the rescue boat first"), FColor::Orange);
        return false;
    }

    const float DistanceToBoat = FVector::Dist(GetActorLocation(), Boat->GetActorLocation());
    if (DistanceToBoat > BoardInteractionRange)
    {
        LastReelResult = TEXT("BOAT TOO FAR");
        ShowDebugMessage(TEXT("Board: move closer to the boat"), FColor::Orange);
        return false;
    }

    if (!Boat->BoardBoat(this))
    {
        LastReelResult = FString::Printf(TEXT("BOAT %s"), *Boat->GetBoatStateText());
        ShowDebugMessage(TEXT("Board: boat is not ready"), FColor::Orange);
        return false;
    }

    BoardedBoat = Boat;
    bIsBoardedBoat = true;
    bIsAiming = false;
    bUseControllerRotationYaw = false;

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->DisableMovement();
        Movement->bOrientRotationToMovement = false;
    }

    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = 520.0f;
    }

    LastReelResult = TEXT("BOARDED BOAT");
    ShowDebugMessage(TEXT("Boarded rescue boat: WASD pilots, F/B exits"), FColor::Cyan);
    return true;
}

bool ADSReelPrototypeCharacter::ExecuteReelActionOnTarget(AActor* Target)
{
    if (!Target || !CanReelPull(Target))
    {
        StartReelFeedback(Target, FColor::Red);
        LastReelResult = TEXT("NO VALID TARGET");
        ShowDebugMessage(TEXT("Reel Pull: no valid target"), FColor::Red);
        return false;
    }

    UpdateTargetMetrics(Target);

    if (ADSPrototypeBoatActor* Boat = Cast<ADSPrototypeBoatActor>(Target))
    {
        StartReelFeedback(Target, FColor::Cyan);
        if (!Boat->ApplyReelTowFrom(GetActorLocation(), Boat->ReelTowStepDistance))
        {
            LastReelResult = FString::Printf(TEXT("BOAT %s"), *Boat->GetBoatStateText());
            ShowDebugMessage(TEXT("Boat Tow: blocked by shallow water or anchor"), FColor::Orange);
            return false;
        }

        ++BoatTowCount;
        LastReelResult = FString::Printf(TEXT("BOAT TOW %d"), BoatTowCount);
        ShowDebugMessage(TEXT("Boat Tow: reel line pulled the boat"), FColor::Cyan);
        return true;
    }

    if (Target->ActorHasTag(TEXT("Civilian")))
    {
        StartReelFeedback(Target, FColor::Green);
        FVector RescueForward = FollowCamera ? FollowCamera->GetForwardVector() : GetActorForwardVector();
        RescueForward.Z = 0.0f;
        RescueForward = RescueForward.GetSafeNormal();
        const FVector RescueLocation = GetActorLocation() + (RescueForward * 150.0f) + FVector(0.0f, 0.0f, 60.0f);
        Target->SetActorLocation(RescueLocation, false, nullptr, ETeleportType::TeleportPhysics);
        ++CivilianRescueCount;
        LastReelResult = FString::Printf(TEXT("RESCUE COMPLETE %d"), CivilianRescueCount);
        ShowDebugMessage(TEXT("Rescue Reel: civilian pulled clear"), FColor::Green);
        return true;
    }

    StartReelFeedback(Target, FColor::Cyan);
    const FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
    FVector LaunchVelocity = ToTarget.GetSafeNormal() * 900.0f;
    LaunchVelocity.Z = 320.0f;
    LaunchCharacter(LaunchVelocity, true, true);
    ++GrapplePullCount;
    LastReelResult = FString::Printf(TEXT("GRAPPLE CAST %d"), GrapplePullCount);
    ShowDebugMessage(TEXT("Grapple Cast: pulling to target"), FColor::Green);
    return true;
}

void ADSReelPrototypeCharacter::UpdateReelLine(float DeltaSeconds)
{
    AActor* Target = AttachedReelTarget.Get();
    if (!Target)
    {
        bReelHoldActive = false;
        RawLineTension = FMath::Max(0.0f, RawLineTension - (ReelTensionReleaseRate * DeltaSeconds));
        LineTension = RawLineTension;
        RefreshReelLineState();
        return;
    }

    UpdateTargetMetrics(Target);

    const float Range = GetTargetInteractionRange(Target);
    const float DistanceRatio = Range > 0.0f ? FMath::Clamp(CurrentTargetDistance / Range, 0.0f, 1.45f) : 0.0f;
    const float WaterPressure = bInWaterZone ? CurrentWaterPressure : 0.0f;

    if (bReelHoldActive)
    {
        RawLineTension += DeltaSeconds * (ReelTensionBuildRate + (DistanceRatio * 0.34f) + (WaterPressure * 0.22f));
        ApplyContinuousReelPull(Target, DeltaSeconds);
    }
    else
    {
        RawLineTension -= DeltaSeconds * ReelTensionReleaseRate;
    }

    RawLineTension = FMath::Clamp(RawLineTension, 0.0f, 1.5f);
    LineTension = FMath::Clamp(RawLineTension, 0.0f, 1.0f);
    RefreshReelLineState();

    if (GetWorld())
    {
        const FVector Start = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation() + FVector(0.0f, 0.0f, 64.0f);
        DrawDebugLine(GetWorld(), Start, Target->GetActorLocation(), GetReelLineStateColor(), false, 0.0f, 0, bReelHoldActive ? 7.0f : 4.0f);
        DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 64.0f, 16, GetReelLineStateColor(), false, 0.0f, 0, 3.0f);
    }

    if (RawLineTension >= ReelSnapThreshold)
    {
        ++ReelSnapCount;
        StartReelFeedback(Target, FColor::Red);
        DetachReelLineInternal(TEXT("LINE SNAP"), true);
    }
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
        if (Distance > GetTargetInteractionRange(Actor))
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

float ADSReelPrototypeCharacter::GetTargetInteractionRange(AActor* Actor) const
{
    if (const UDSTargetableComponent* Targetable = GetTargetableComponent(Actor))
    {
        return FMath::Max(Targetable->InteractionRange, 1.0f);
    }

    return 1200.0f;
}

void ADSReelPrototypeCharacter::UpdateTargetMetrics(AActor* Actor)
{
    if (!Actor)
    {
        CurrentTargetDistance = 0.0f;
        LineTension = 0.0f;
        return;
    }

    CurrentTargetDistance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
    const float Range = GetTargetInteractionRange(Actor);
    LineTension = Range > 0.0f ? FMath::Clamp(CurrentTargetDistance / Range, 0.0f, 1.0f) : 0.0f;
}

void ADSReelPrototypeCharacter::UpdateWaterState(float DeltaSeconds)
{
    bInWaterZone = false;
    bInBoatableWater = false;
    CurrentWaterDepth = 0.0f;
    WaterMovementScale = 1.0f;
    CurrentWaterPressure = 0.0f;
    CurrentWaterVelocity = FVector::ZeroVector;
    CurrentWaterStateText = TEXT("DRY");

    for (TActorIterator<ADSWaterZoneActor> It(GetWorld()); It; ++It)
    {
        const ADSWaterZoneActor* WaterZone = *It;
        if (!WaterZone || !WaterZone->ContainsWorldLocation(GetActorLocation()))
        {
            continue;
        }

        const float Depth = WaterZone->GetDepthAtLocation(GetActorLocation());
        if (!bInWaterZone || Depth > CurrentWaterDepth)
        {
            bInWaterZone = true;
            CurrentWaterDepth = Depth;
            WaterMovementScale = WaterZone->GetMovementScaleAtLocation(GetActorLocation());
            bInBoatableWater = WaterZone->CanBoatOperateAtLocation(GetActorLocation());
            CurrentWaterPressure = WaterZone->GetReelTensionPressureAtLocation(GetActorLocation());
            CurrentWaterVelocity = WaterZone->GetCurrentVelocityAtLocation(GetActorLocation());
            CurrentWaterStateText = WaterZone->GetWaterStateTextAtLocation(GetActorLocation());
        }
    }

    ApplyMovementSpeed();
    ApplyWaterCurrent(DeltaSeconds);
}

void ADSReelPrototypeCharacter::ApplyMovementSpeed()
{
    UCharacterMovementComponent* Movement = GetCharacterMovement();
    if (!Movement)
    {
        return;
    }

    const float BaseSpeed = bIsAiming ? 360.0f : 500.0f;
    Movement->MaxWalkSpeed = BaseSpeed * WaterMovementScale;
}

void ADSReelPrototypeCharacter::ApplyWaterCurrent(float DeltaSeconds)
{
    if (!bInWaterZone || bIsBoardedBoat || DeltaSeconds <= 0.0f || CurrentWaterVelocity.IsNearlyZero())
    {
        return;
    }

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        const float CurrentInfluence = FMath::Clamp(CurrentWaterPressure, 0.0f, 1.0f) * 0.22f;
        Movement->Velocity += CurrentWaterVelocity * CurrentInfluence * DeltaSeconds;
    }
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

    return Actor->ActorHasTag(TEXT("GrapplePoint")) || Actor->ActorHasTag(TEXT("Civilian")) || Actor->ActorHasTag(TEXT("Boat"));
}

void ADSReelPrototypeCharacter::ApplyContinuousReelPull(AActor* Target, float DeltaSeconds)
{
    if (!Target || DeltaSeconds <= 0.0f)
    {
        return;
    }

    if (ADSPrototypeBoatActor* Boat = Cast<ADSPrototypeBoatActor>(Target))
    {
        if (Boat->ApplyReelTowFrom(GetActorLocation(), ReelBoatTowSpeed * DeltaSeconds))
        {
            LastReelResult = TEXT("BOAT REELING");
        }
        else
        {
            LastReelResult = FString::Printf(TEXT("BOAT %s"), *Boat->GetBoatStateText());
        }
        return;
    }

    if (Target->ActorHasTag(TEXT("Civilian")))
    {
        FVector PullDirection = GetActorLocation() - Target->GetActorLocation();
        PullDirection.Z = 0.0f;

        const float DistanceToCivilian = PullDirection.Size();
        if (DistanceToCivilian <= ReelRescueCompleteDistance)
        {
            StartReelFeedback(Target, FColor::Green);
            Target->SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 120.0f) + FVector(0.0f, 0.0f, 60.0f), false, nullptr, ETeleportType::TeleportPhysics);
            ++CivilianRescueCount;
            DetachReelLineInternal(FString::Printf(TEXT("RESCUE COMPLETE %d"), CivilianRescueCount), false);
            ShowDebugMessage(TEXT("Rescue Reel: civilian pulled clear"), FColor::Green);
            return;
        }

        if (PullDirection.Normalize())
        {
            const float PullStep = ReelTargetPullSpeed * DeltaSeconds * FMath::Clamp(1.0f - (RawLineTension * 0.28f), 0.45f, 1.0f);
            Target->AddActorWorldOffset(PullDirection * PullStep, false, nullptr, ETeleportType::TeleportPhysics);
            LastReelResult = TEXT("RESCUE REELING");
        }
        return;
    }

    FVector PullDirection = Target->GetActorLocation() - GetActorLocation();
    PullDirection.Z = 0.0f;
    if (!PullDirection.Normalize())
    {
        return;
    }

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        const float TensionEfficiency = RawLineTension > ReelDangerThreshold ? 0.58f : (RawLineTension > ReelStrainThreshold ? 0.86f : 1.0f);
        Movement->Velocity += PullDirection * ReelSelfPullAcceleration * TensionEfficiency * DeltaSeconds;
        if (CurrentTargetDistance > 280.0f)
        {
            Movement->Velocity.Z = FMath::Max(Movement->Velocity.Z, 180.0f);
        }
    }

    LastReelResult = RawLineTension > ReelDangerThreshold ? TEXT("REEL DANGER") : TEXT("GRAPPLE REELING");

    if (CurrentTargetDistance <= 180.0f)
    {
        ++GrapplePullCount;
        DetachReelLineInternal(FString::Printf(TEXT("GRAPPLE COMPLETE %d"), GrapplePullCount), false);
    }
}

void ADSReelPrototypeCharacter::DetachReelLineInternal(const FString& ResultText, bool bWasSnap)
{
    AttachedReelTarget.Reset();
    bReelHoldActive = false;
    LastReelResult = ResultText;

    if (bWasSnap)
    {
        RawLineTension = 1.0f;
        LineTension = 1.0f;
        ReelLineStateText = TEXT("SNAPPED");
        ShowDebugMessage(TEXT("Line snapped under tension"), FColor::Red);
        return;
    }

    RawLineTension = 0.0f;
    LineTension = 0.0f;
    ReelLineStateText = TEXT("IDLE");
}

void ADSReelPrototypeCharacter::RefreshReelLineState()
{
    if (!AttachedReelTarget.IsValid())
    {
        if (ReelLineStateText == TEXT("SNAPPED") && RawLineTension <= 0.02f)
        {
            ReelLineStateText = TEXT("IDLE");
        }
        else if (ReelLineStateText != TEXT("SNAPPED"))
        {
            ReelLineStateText = TEXT("IDLE");
        }
        return;
    }

    if (RawLineTension >= ReelDangerThreshold)
    {
        ReelLineStateText = TEXT("DANGER");
    }
    else if (RawLineTension >= ReelStrainThreshold)
    {
        ReelLineStateText = TEXT("STRAIN");
    }
    else if (bReelHoldActive)
    {
        ReelLineStateText = TEXT("REELING");
    }
    else
    {
        ReelLineStateText = TEXT("CAST");
    }
}

FColor ADSReelPrototypeCharacter::GetReelLineStateColor() const
{
    if (ReelLineStateText == TEXT("DANGER") || ReelLineStateText == TEXT("SNAPPED"))
    {
        return FColor::Red;
    }

    if (ReelLineStateText == TEXT("STRAIN"))
    {
        return FColor::Yellow;
    }

    return bReelHoldActive ? FColor::Green : FColor::Cyan;
}

void ADSReelPrototypeCharacter::ExitBoardedBoat()
{
    if (ADSPrototypeBoatActor* Boat = BoardedBoat.Get())
    {
        Boat->ExitBoat();
    }

    BoardedBoat.Reset();
    bIsBoardedBoat = false;
    bUseControllerRotationYaw = false;

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->SetMovementMode(MOVE_Walking);
        Movement->bOrientRotationToMovement = true;
    }

    ApplyMovementSpeed();

    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = 450.0f;
    }

    LastReelResult = TEXT("EXITED BOAT");
    ShowDebugMessage(TEXT("Exited rescue boat"), FColor::Cyan);
}

void ADSReelPrototypeCharacter::StartReelFeedback(AActor* Target, const FColor& Color)
{
    const FVector Start = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation() + FVector(0.0f, 0.0f, 64.0f);
    const FVector Forward = FollowCamera ? FollowCamera->GetForwardVector() : GetActorForwardVector();
    const FVector End = Target ? Target->GetActorLocation() : Start + (Forward * 700.0f);

    LastReelFeedbackStart = Start;
    LastReelFeedbackEnd = End;
    LastReelFeedbackColor = Color;
    ReelFeedbackTimeRemaining = 0.35f;
}

void ADSReelPrototypeCharacter::DrawReelFeedback(float DeltaSeconds)
{
    if (ReelFeedbackTimeRemaining <= 0.0f || !GetWorld())
    {
        return;
    }

    constexpr float MaxFeedbackTime = 0.35f;
    ReelFeedbackTimeRemaining = FMath::Max(0.0f, ReelFeedbackTimeRemaining - DeltaSeconds);
    const float AgeAlpha = 1.0f - (ReelFeedbackTimeRemaining / MaxFeedbackTime);
    const float PulseRadius = 52.0f + (AgeAlpha * 28.0f);

    DrawDebugLine(GetWorld(), LastReelFeedbackStart, LastReelFeedbackEnd, LastReelFeedbackColor, false, 0.0f, 0, 6.0f);
    DrawDebugSphere(GetWorld(), LastReelFeedbackEnd, PulseRadius, 16, LastReelFeedbackColor, false, 0.0f, 0, 3.0f);
}

void ADSReelPrototypeCharacter::ShowDebugMessage(const FString& Message, const FColor& Color) const
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(81057, 0.0f, Color, Message);
    }
}
