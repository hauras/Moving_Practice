#include "Player/FlyPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AFlyPlayer::AFlyPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    // ��� ���� �̵� �ӵ��� ȸ�� �ӵ� ����
    MoveSpeed = 100.0f;
}

void AFlyPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Roll ���� ���������� �ٿ� �ڿ������� ȸ�� ���� ����
    if (FMath::IsNearlyZero(RotationInput.X, 0.01f))
    {
        RotationInput.X = 0.0f; // Roll �� ������ ����
    }
    else
    {
        RotationInput.X = FMath::FInterpTo(RotationInput.X, 0.0f, DeltaTime, 5.0f); // ���� ó��
    }

    // ȸ�� ó��
    CurrentRotation.Pitch += RotationInput.Y * RotationSpeed * DeltaTime;
    CurrentRotation.Yaw += RotationInput.Z * RotationSpeed * DeltaTime;
    CurrentRotation.Roll += RotationInput.X * RotationSpeed * DeltaTime;
    SetActorRotation(CurrentRotation);

    // ���� ��ǥ�� ��� �̵� ó��
    FVector Forward = GetActorForwardVector() * Velocity.X;
    FVector Right = GetActorRightVector() * Velocity.Y;
    FVector Up = GetActorUpVector() * Velocity.Z; // ����� Up ���� ��� (�ٶ󺸴� ����)

    FVector Movement = (Forward + Right + Up) * MoveSpeed * DeltaTime;
    AddActorLocalOffset(Movement, true);
}

void AFlyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // ���� �Է� ���ε�
        EnhancedInput->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &AFlyPlayer::MoveUp);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFlyPlayer::Look);
        EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &AFlyPlayer::Roll);
    }
}

void AFlyPlayer::MoveUp(const FInputActionValue& Value)
{
    float Up = Value.Get<float>();


    // �Է� ���� 0�� �� ����
    if (FMath::IsNearlyZero(Up))
    {
        Velocity.Z = 0.0f; // Z�� �̵� ����
    }
    else
    {
        Velocity.Z = Up; // Z�� �̵� �� ���� (���� Up ���Ϳ� ���� ����)
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