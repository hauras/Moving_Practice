#include "Player/MovingPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"

AMovingPlayer::AMovingPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    // ĸ�� ������Ʈ ����
    CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CollisionCapsule->InitCapsuleSize(30.f, 40.f);
    SetRootComponent(CollisionCapsule);

    // ���̷�Ż �޽� ����
    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh->SetupAttachment(CollisionCapsule);

    // ������ �� ����
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CollisionCapsule);
    SpringArm->TargetArmLength = 300.f;
    SpringArm->bUsePawnControlRotation = true;

    // ī�޶� ����
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(SpringArm);

    bUseControllerRotationYaw = false;

    // �⺻ �ӵ� �� ����
    Speed = 600.f;
    RotationSpeed = 100.f;
    Velocity = FVector::ZeroVector;
}

void AMovingPlayer::BeginPlay()
{
    Super::BeginPlay();

    // Enhanced Input Mapping Context ����
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }
}

void AMovingPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // �̵� ó��
    if (!Velocity.IsNearlyZero())
    {
        AddActorLocalOffset(Velocity * DeltaTime, true);
    }
}

void AMovingPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveForwardAction)
        {
            EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AMovingPlayer::MoveForward);
            EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Completed, this, &AMovingPlayer::StopMovement);
        }

        if (MoveRightAction)
        {
            EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AMovingPlayer::MoveRight);
            EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &AMovingPlayer::StopMovement);
        }

        if (LookAction)
        {
            EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMovingPlayer::Look);
        }
    }
}

void AMovingPlayer::MoveForward(const FInputActionValue& Value)
{
    float ForwardValue = Value.Get<float>();
    Velocity.X = ForwardValue * Speed;
}

void AMovingPlayer::MoveRight(const FInputActionValue& Value)
{
    float RightValue = Value.Get<float>();
    Velocity.Y = RightValue * Speed;
}

void AMovingPlayer::Look(const FInputActionValue& Value)
{
    FVector2D LookAxis = Value.Get<FVector2D>();

    // ī�޶� ȸ�� �߰�
    if (Controller)
    {
        AddControllerYawInput(LookAxis.X * RotationSpeed * GetWorld()->GetDeltaSeconds()); // �¿� ȸ�� (Yaw)
        AddControllerPitchInput(LookAxis.Y * RotationSpeed * GetWorld()->GetDeltaSeconds()); // ���� ȸ�� (Pitch)
    }
}

void AMovingPlayer::StopMovement(const FInputActionValue& Value)
{
    Velocity = FVector::ZeroVector;  // �Է��� ������ ����
}