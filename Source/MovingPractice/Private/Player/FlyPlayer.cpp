#include "Player/FlyPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AFlyPlayer::AFlyPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    // 드론 전용 이동 속도와 회전 속도 설정
    MoveSpeed = 100.0f;
}

void AFlyPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Roll 값을 점진적으로 줄여 자연스러운 회전 감속 구현
    if (FMath::IsNearlyZero(RotationInput.X, 0.01f))
    {
        RotationInput.X = 0.0f; // Roll 값 완전히 멈춤
    }
    else
    {
        RotationInput.X = FMath::FInterpTo(RotationInput.X, 0.0f, DeltaTime, 5.0f); // 감속 처리
    }

    // 회전 처리
    CurrentRotation.Pitch += RotationInput.Y * RotationSpeed * DeltaTime;
    CurrentRotation.Yaw += RotationInput.Z * RotationSpeed * DeltaTime;
    CurrentRotation.Roll += RotationInput.X * RotationSpeed * DeltaTime;
    SetActorRotation(CurrentRotation);

    // 로컬 좌표계 기반 이동 처리
    FVector Forward = GetActorForwardVector() * Velocity.X;
    FVector Right = GetActorRightVector() * Velocity.Y;
    FVector Up = GetActorUpVector() * Velocity.Z; // 드론의 Up 벡터 사용 (바라보는 방향)

    FVector Movement = (Forward + Right + Up) * MoveSpeed * DeltaTime;
    AddActorLocalOffset(Movement, true);
}

void AFlyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 비행 입력 바인딩
        EnhancedInput->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &AFlyPlayer::MoveUp);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFlyPlayer::Look);
        EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &AFlyPlayer::Roll);
    }
}

void AFlyPlayer::MoveUp(const FInputActionValue& Value)
{
    float Up = Value.Get<float>();


    // 입력 값이 0일 때 정지
    if (FMath::IsNearlyZero(Up))
    {
        Velocity.Z = 0.0f; // Z축 이동 멈춤
    }
    else
    {
        Velocity.Z = Up; // Z축 이동 값 저장 (로컬 Up 벡터에 따라 적용)
    }
}

void AFlyPlayer::Look(const FInputActionValue& Value)
{
    FVector2D LookInput = Value.Get<FVector2D>();
    RotationInput.Y = LookInput.Y; // Pitch
    RotationInput.Z = LookInput.X; // Yaw
}

void AFlyPlayer::Roll(const FInputActionValue& Value)
{
    float Roll = Value.Get<float>();
    RotationInput.X = Roll; // Roll
}