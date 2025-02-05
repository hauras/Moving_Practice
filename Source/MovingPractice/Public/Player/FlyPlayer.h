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

 
    void MoveUp(const FInputActionValue& Value);    // 惑铰/窍碍
    void Look(const FInputActionValue& Value);      // Yaw/Pitch 贸府
    void Roll(const FInputActionValue& Value);      // Roll 贸府

private:
 
    FVector RotationInput;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 10.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveUpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> RollAction; 
};