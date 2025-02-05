#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "MovingPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class MOVINGPRACTICE_API AMovingPlayer : public APawn
{
    GENERATED_BODY()

public:
    AMovingPlayer();

    void MoveForward(const FInputActionValue& Value);
    void MoveRight(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StopMovement(const FInputActionValue& Value); // ���Ӱ� �߰��� ���� �Լ�

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UCapsuleComponent* CollisionCapsule;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent* SkeletalMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    // �Է� ����
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> InputMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveForwardAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveRightAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> LookAction;

    // �̵� ���� ����
    UPROPERTY(EditAnywhere, Category = "Movement")
    float Speed = 600.f; // �⺻ �̵� �ӵ��� 600���� ����

    // ���� �̵� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    FVector Velocity = FVector::ZeroVector; // �ʱ� �̵����� 0���� ����

    FRotator CurrentRotation;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float RotationSpeed = 100.f; // ȸ�� �ӵ� ����
};
