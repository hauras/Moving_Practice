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

    // 드론 전용 입력 처리
    void MoveUp(const FInputActionValue& Value);    // 상승/하강
    void Look(const FInputActionValue& Value);      // Yaw/Pitch 처리
    void Roll(const FInputActionValue& Value);      // Roll 처리

private:
    // 추가 입력 값 관리
    FVector RotationInput;

    // 드론 이동 및 회전 속도
    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 10.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveUpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> RollAction; // Roll 입력 처리용
};