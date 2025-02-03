#include "Player/FlyPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AFlyPlayer::AFlyPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    // ��� ���� �̵� �ӵ��� ȸ�� �ӵ� ����
    MoveSpeed = 10.0f;
    RotationSpeed = 100.0f;
}

void AFlyPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // �θ� Ŭ������ CurrentRotation ���
    CurrentRotation.Pitch += RotationInput.Y * RotationSpeed * DeltaTime;
    CurrentRotation.Yaw += RotationInput.Z * RotationSpeed * DeltaTime;
    CurrentRotation.Roll += RotationInput.X * RotationSpeed * DeltaTime;
    SetActorRotation(CurrentRotation);

    // �̵� ó��
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
        // ���� �Է� ���ε� ����
        EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AMovingPlayer::MoveForward);
        EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AMovingPlayer::MoveRight);

        // ��� ���� �Է� ���ε� �߰�
        EnhancedInput->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &AFlyPlayer::MoveUp);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFlyPlayer::Look);
        EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &AFlyPlayer::Roll);
    }
}

void AFlyPlayer::MoveUp(const FInputActionValue& Value)
{
    float UpValue = Value.Get<float>();
    Velocity.Z = UpValue * MoveSpeed; // Z�� �̵� (���/�ϰ�)
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