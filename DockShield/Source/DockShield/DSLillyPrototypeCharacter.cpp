#include "DSLillyPrototypeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "DSTargetableComponent.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputCoreTypes.h"
#include "InputMappingContext.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

namespace
{
void ConfigureLillyPrototypeMesh(UStaticMeshComponent* Component, UStaticMesh* Mesh, const FVector& Location, const FRotator& Rotation, const FVector& Scale)
{
    if (!Component)
    {
        return;
    }

    Component->SetStaticMesh(Mesh);
    Component->SetRelativeLocation(Location);
    Component->SetRelativeRotation(Rotation);
    Component->SetRelativeScale3D(Scale);
    Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Component->SetCastShadow(true);
    Component->SetRenderCustomDepth(true);
    Component->SetCustomDepthStencilValue(6);
}

void ApplyLillyPrototypeColor(UStaticMeshComponent* Component, UObject* Owner, const FLinearColor& Color, float Metallic, float Roughness)
{
    if (!Component)
    {
        return;
    }

    if (UMaterialInterface* BaseMaterial = Component->GetMaterial(0))
    {
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, Owner);
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
        DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), Color);
        DynamicMaterial->SetVectorParameterValue(TEXT("Tint"), Color);
        DynamicMaterial->SetScalarParameterValue(TEXT("Metallic"), Metallic);
        DynamicMaterial->SetScalarParameterValue(TEXT("Roughness"), Roughness);
        Component->SetMaterial(0, DynamicMaterial);
    }
}
}

ADSLillyPrototypeCharacter::ADSLillyPrototypeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(38.0f, 88.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->bOrientRotationToMovement = true;
    Movement->RotationRate = FRotator(0.0f, 380.0f, 0.0f);
    Movement->JumpZVelocity = 430.0f;
    Movement->AirControl = 0.34f;
    Movement->MaxWalkSpeed = 405.0f;
    Movement->MinAnalogWalkSpeed = 20.0f;
    Movement->BrakingDecelerationWalking = 1700.0f;
    Movement->BrakingDecelerationFalling = 1300.0f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 420.0f;
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
        GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -83.0f));
        GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
        GetMesh()->SetRelativeScale3D(FVector(0.88f, 0.88f, 0.92f));
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimFinder(TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"));
    if (AnimFinder.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimFinder.Class);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));

    LillyRootStaffMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LillyRootStaffMesh"));
    LillyRootStaffMesh->SetupAttachment(RootComponent);
    ConfigureLillyPrototypeMesh(LillyRootStaffMesh, CylinderMeshFinder.Object, FVector(18.0f, 50.0f, 58.0f), FRotator(0.0f, 0.0f, 18.0f), FVector(0.04f, 0.04f, 0.68f));

    LillyVineLashMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LillyVineLashMesh"));
    LillyVineLashMesh->SetupAttachment(RootComponent);
    ConfigureLillyPrototypeMesh(LillyVineLashMesh, CylinderMeshFinder.Object, FVector(40.0f, -44.0f, 58.0f), FRotator(54.0f, 0.0f, 84.0f), FVector(0.025f, 0.025f, 0.54f));

    LillyShoulderBloomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LillyShoulderBloomMesh"));
    LillyShoulderBloomMesh->SetupAttachment(RootComponent);
    ConfigureLillyPrototypeMesh(LillyShoulderBloomMesh, SphereMeshFinder.Object, FVector(28.0f, -26.0f, 112.0f), FRotator::ZeroRotator, FVector(0.10f, 0.10f, 0.07f));

    LillySwampCrownMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LillySwampCrownMesh"));
    LillySwampCrownMesh->SetupAttachment(RootComponent);
    ConfigureLillyPrototypeMesh(LillySwampCrownMesh, CubeMeshFinder.Object, FVector(32.0f, 0.0f, 126.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(0.16f, 0.04f, 0.035f));
}

void ADSLillyPrototypeCharacter::BeginPlay()
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

    ApplyPrototypeVisualStyle();
}

void ADSLillyPrototypeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    CurrentBindTarget = FindBestBindTarget();
    DrawRootDebug();

    if (AActor* Target = CurrentBindTarget.Get())
    {
        const FColor TargetColor = CanLillyBind(Target) ? FColor(120, 255, 70) : FColor::Orange;
        DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 70.0f, 16, TargetColor, false, 0.0f, 0, 3.0f);
        ShowDebugMessage(GetCurrentBindPrompt(), TargetColor);
    }
    else
    {
        ShowDebugMessage(TEXT("Lilly: Q swamp pulse, E/LMB bind target"), FColor(120, 255, 70));
    }
}

void ADSLillyPrototypeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADSLillyPrototypeCharacter::Move);
        }
        if (LookAction)
        {
            EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADSLillyPrototypeCharacter::Look);
        }
        if (JumpAction)
        {
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        }
    }

    PlayerInputComponent->BindKey(EKeys::Q, IE_Pressed, this, &ADSLillyPrototypeCharacter::HandleSwampPulse);
    PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &ADSLillyPrototypeCharacter::TryLillyBind);
    PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ADSLillyPrototypeCharacter::TryLillyBind);
    PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &ADSLillyPrototypeCharacter::StartAim);
    PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Released, this, &ADSLillyPrototypeCharacter::StopAim);
}

int32 ADSLillyPrototypeCharacter::SwampPulse()
{
    LastSwampPulseHitCount = 0;

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor || Actor == this || !CanLillyBind(Actor))
        {
            continue;
        }

        const float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
        if (Distance > BindRange || !IsInsideRootFocus(Actor, -0.25f))
        {
            continue;
        }

        ++LastSwampPulseHitCount;
        DrawDebugSphere(GetWorld(), Actor->GetActorLocation(), 84.0f, 20, FColor(120, 255, 70), false, 1.2f, 0, 4.0f);
    }

    LastBindResult = FString::Printf(TEXT("SWAMP PULSE %d"), LastSwampPulseHitCount);
    ShowDebugMessage(LastBindResult, FColor(120, 255, 70));
    return LastSwampPulseHitCount;
}

bool ADSLillyPrototypeCharacter::ExecuteLillyBindOnTarget(AActor* Target)
{
    UDSTargetableComponent* Targetable = GetTargetableComponent(Target);
    if (!Target || !Targetable)
    {
        LastBindResult = TEXT("NO BIND TARGET");
        ShowDebugMessage(TEXT("Lilly Bind: no target"), FColor::Red);
        return false;
    }

    Targetable->ConfigureFromOwnerTags();
    if (!Targetable->CanLillyBind())
    {
        LastBindResult = TEXT("TARGET NOT BINDABLE");
        ShowDebugMessage(TEXT("Lilly Bind: target cannot be bound"), FColor::Orange);
        return false;
    }

    const bool bWasBound = Targetable->IsLillyBound();
    const float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
    const float Strength = 1.0f - FMath::Clamp(Distance / FMath::Max(BindRange, 1.0f), 0.0f, 0.68f);
    if (!Targetable->BindForLilly(Strength))
    {
        LastBindResult = TEXT("BIND FAILED");
        return false;
    }

    if (!bWasBound)
    {
        ++BoundTargetCount;
    }

    LastBindResult = FString::Printf(TEXT("LILLY BIND %d"), BoundTargetCount);
    DrawDebugLine(GetWorld(), FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation(), Target->GetActorLocation(), FColor(120, 255, 70), false, 0.75f, 0, 5.0f);
    DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 98.0f, 24, FColor(160, 255, 90), false, 0.75f, 0, 4.0f);
    ShowDebugMessage(Targetable->GetLillyPrompt(), FColor(120, 255, 70));
    return true;
}

void ADSLillyPrototypeCharacter::TryLillyBind()
{
    ExecuteLillyBindOnTarget(CurrentBindTarget.Get());
}

AActor* ADSLillyPrototypeCharacter::GetCurrentBindTargetActor() const
{
    return CurrentBindTarget.Get();
}

FString ADSLillyPrototypeCharacter::GetCurrentBindPrompt() const
{
    if (AActor* Target = CurrentBindTarget.Get())
    {
        if (UDSTargetableComponent* Targetable = GetTargetableComponent(Target))
        {
            return Targetable->GetLillyPrompt();
        }
        return FString::Printf(TEXT("Bind Target: %s"), *Target->GetName());
    }

    return TEXT("No bind target: pulse swamp sense or face a DockShield target");
}

bool ADSLillyPrototypeCharacter::IsCurrentBindTargetValid() const
{
    return CanLillyBind(CurrentBindTarget.Get());
}

bool ADSLillyPrototypeCharacter::IsAiming() const
{
    return bIsAiming;
}

float ADSLillyPrototypeCharacter::GetBindRange() const
{
    return BindRange;
}

float ADSLillyPrototypeCharacter::GetRootRadius() const
{
    return RootRadius;
}

int32 ADSLillyPrototypeCharacter::GetLastSwampPulseHitCount() const
{
    return LastSwampPulseHitCount;
}

int32 ADSLillyPrototypeCharacter::GetBoundTargetCount() const
{
    return BoundTargetCount;
}

FString ADSLillyPrototypeCharacter::GetLastBindResult() const
{
    return LastBindResult;
}

int32 ADSLillyPrototypeCharacter::GetPrototypeVisualKitComponentCount() const
{
    int32 Count = 0;
    Count += LillyRootStaffMesh && LillyRootStaffMesh->GetStaticMesh() ? 1 : 0;
    Count += LillyVineLashMesh && LillyVineLashMesh->GetStaticMesh() ? 1 : 0;
    Count += LillyShoulderBloomMesh && LillyShoulderBloomMesh->GetStaticMesh() ? 1 : 0;
    Count += LillySwampCrownMesh && LillySwampCrownMesh->GetStaticMesh() ? 1 : 0;
    return Count;
}

FString ADSLillyPrototypeCharacter::GetPrototypeVisualProfile() const
{
    return TEXT("LILLY LOCH: swamp queen, root staff, vine lash, green bloom marker, petite silhouette");
}

void ADSLillyPrototypeCharacter::Move(const FInputActionValue& Value)
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

void ADSLillyPrototypeCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void ADSLillyPrototypeCharacter::HandleSwampPulse()
{
    SwampPulse();
}

void ADSLillyPrototypeCharacter::StartAim()
{
    bIsAiming = true;
    bUseControllerRotationYaw = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = 255.0f;
    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = 345.0f;
    }
}

void ADSLillyPrototypeCharacter::StopAim()
{
    bIsAiming = false;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->MaxWalkSpeed = 405.0f;
    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = 420.0f;
    }
}

AActor* ADSLillyPrototypeCharacter::FindBestBindTarget() const
{
    AActor* BestTarget = nullptr;
    float BestScore = TNumericLimits<float>::Max();

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor || Actor == this || !CanLillyBind(Actor))
        {
            continue;
        }

        const float Distance = FVector::Dist(FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation(), Actor->GetActorLocation());
        if (Distance > BindRange || !IsInsideRootFocus(Actor, bIsAiming ? 0.28f : 0.04f))
        {
            continue;
        }

        const FVector ViewForward = FollowCamera ? FollowCamera->GetForwardVector() : GetActorForwardVector();
        const FVector ToActor = (Actor->GetActorLocation() - (FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation())).GetSafeNormal();
        const float AimDot = FVector::DotProduct(ViewForward, ToActor);
        const float Score = Distance - (AimDot * 480.0f);
        if (Score < BestScore)
        {
            BestScore = Score;
            BestTarget = Actor;
        }
    }

    return BestTarget;
}

UDSTargetableComponent* ADSLillyPrototypeCharacter::GetTargetableComponent(AActor* Actor) const
{
    return Actor ? Actor->FindComponentByClass<UDSTargetableComponent>() : nullptr;
}

bool ADSLillyPrototypeCharacter::CanLillyBind(AActor* Actor) const
{
    if (UDSTargetableComponent* Targetable = GetTargetableComponent(Actor))
    {
        Targetable->ConfigureFromOwnerTags();
        return Targetable->CanLillyBind();
    }

    return false;
}

bool ADSLillyPrototypeCharacter::IsInsideRootFocus(AActor* Actor, float MinDot) const
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

void ADSLillyPrototypeCharacter::ApplyPrototypeVisualStyle()
{
    ApplyLillyPrototypeColor(LillyRootStaffMesh, this, FLinearColor(0.16f, 0.10f, 0.045f, 1.0f), 0.25f, 0.42f);
    ApplyLillyPrototypeColor(LillyVineLashMesh, this, FLinearColor(0.10f, 0.34f, 0.06f, 1.0f), 0.12f, 0.52f);
    ApplyLillyPrototypeColor(LillyShoulderBloomMesh, this, FLinearColor(0.58f, 1.0f, 0.16f, 1.0f), 0.18f, 0.18f);
    ApplyLillyPrototypeColor(LillySwampCrownMesh, this, FLinearColor(0.38f, 0.16f, 0.07f, 1.0f), 0.2f, 0.4f);
}

void ADSLillyPrototypeCharacter::DrawRootDebug() const
{
    if (!GetWorld())
    {
        return;
    }

    const FVector Start = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation() + FVector(0.0f, 0.0f, 64.0f);
    const FVector Forward = FollowCamera ? FollowCamera->GetForwardVector() : GetActorForwardVector();
    const FColor RootColor = bIsAiming ? FColor(120, 255, 70) : FColor(94, 170, 60);

    DrawDebugLine(GetWorld(), Start, Start + (Forward * BindRange), RootColor, false, 0.0f, 0, 1.5f);
    DrawDebugSphere(GetWorld(), GetActorLocation(), RootRadius, 24, FColor(64, 130, 46), false, 0.0f, 0, 1.0f);
}

void ADSLillyPrototypeCharacter::ShowDebugMessage(const FString& Message, const FColor& Color) const
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(81059, 0.0f, Color, Message);
    }
}
