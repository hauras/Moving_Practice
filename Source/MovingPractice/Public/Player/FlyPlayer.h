#pragma once

#include "CoreMinimal.h"
#include "Player/MovingPlayer.h"
#include "FlyPlayer.generated.h"

UCLASS()
class MOVINGPRACTICE_API AFlyPlayer : public AMovingPlayer
{
    GENERATED_BODY()

public:
    AFlyPlayer();

protected:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // ��� ���� �Է� ó��
    void MoveUp(const FInputActionValue& Value);    // ���/�ϰ�
    void Look(const FInputActionValue& Value);      // Yaw/Pitch ó��
    void Roll(const FInputActionValue& Value);      // Roll ó��

private:
    // �߰� �Է� �� ����
    FVector RotationInput;

    // ��� �̵� �� ȸ�� �ӵ�
    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 10.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveUpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> RollAction; // Roll �Է� ó����
};