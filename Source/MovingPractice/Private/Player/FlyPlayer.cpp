#include "Player/FlyPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AFlyPlayer::AFlyPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    // 드론 전용 이동 속도와 회전 속도 설정
    MoveSpeed = 10.0f;
    RotationSpeed = 100.0f;
}

void AFlyPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 부모 클래스의 CurrentRotation 사용
    CurrentRotation.Pitch += RotationInput.Y * RotationSpeed * DeltaTime;
    CurrentRotation.Yaw += RotationInput.Z * RotationSpeed * DeltaTime;
    CurrentRotation.Roll += RotationInput.X * RotationSpeed * DeltaTime;
    SetActorRotation(CurrentRotation);

    // 이동 처리
    FVector Forward = GetActorForwardVector() * Velocity.X;
    FVector Right = GetActorRightVector() * Velocity.Y;
    FVector Up = FVector::UpVector * Velocity.Z;

    FVector Movement = (Forward + Right + Up) * MoveSpeed * DeltaTime;
    AddActorLocalOffset(Movement, true);
}

void AFlyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 기존 입력 바인딩 유지
        EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AMovingPlayer::MoveForward);
        EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AMovingPlayer::MoveRight);

        // 드론 전용 입력 바인딩 추가
        EnhancedInput->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &AFlyPlayer::MoveUp);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFlyPlayer::Look);
        EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &AFlyPlayer::Roll);
    }
}

void AFlyPlayer::MoveUp(const FInputActionValue& Value)
{
    float UpValue = Value.Get<float>();
    Velocity.Z = UpValue * MoveSpeed; // Z축 이동 (상승/하강)
}

void AFlyPlayer::Look(const FInputActionValue& Value)
{
    FVector2D LookInput = Value.Get<FVector2D>();
    RotationInput.Y = LookInput.Y; // Pitch
    RotationInput.Z = LookInput.X; // Yaw
}

void AFlyPlayer::Roll(const FInputActionValue& Value)
{
    float RollValue = Value.Get<float>();
    RotationInput.X = RollValue; // Roll
}