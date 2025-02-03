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

    // 캡슐 컴포넌트 설정
    CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CollisionCapsule->InitCapsuleSize(30.f, 40.f);
    SetRootComponent(CollisionCapsule);

    // 스켈레탈 메시 설정
    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh->SetupAttachment(CollisionCapsule);

    // 스프링 암 설정
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CollisionCapsule);
    SpringArm->TargetArmLength = 300.f;
    SpringArm->bUsePawnControlRotation = true;

    // 카메라 설정
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(SpringArm);

    bUseControllerRotationYaw = false;

    // 기본 속도 값 설정
    Speed = 600.f;
    RotationSpeed = 100.f;
    Velocity = FVector::ZeroVector;
}

void AMovingPlayer::BeginPlay()
{
    Super::BeginPlay();

    // Enhanced Input Mapping Context 설정
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

    // 이동 처리
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

    // 카메라 회전 추가
    if (Controller)
    {
        AddControllerYawInput(LookAxis.X * RotationSpeed * GetWorld()->GetDeltaSeconds()); // 좌우 회전 (Yaw)
        AddControllerPitchInput(LookAxis.Y * RotationSpeed * GetWorld()->GetDeltaSeconds()); // 상하 회전 (Pitch)
    }
}

void AMovingPlayer::StopMovement(const FInputActionValue& Value)
{
    Velocity = FVector::ZeroVector;  // 입력이 끝나면 정지
}