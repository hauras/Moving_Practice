#include "AnimInstance/MoveAnimInstance.h"
#include "Player/MovingPlayer.h"

void UMoveAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // APawn 기반의 소유자 가져오기
    OwningPawn = Cast<AMovingPlayer>(TryGetPawnOwner());
}

void UMoveAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

    if (!OwningPawn)
    {
        OwningPawn = Cast<AMovingPlayer>(TryGetPawnOwner());
    }

    if (OwningPawn)
    {
        FVector Velocity = OwningPawn->GetVelocity();
        GroundSpeed = Velocity.Size2D();
        bHasAcceleration = !Velocity.IsNearlyZero(0.1f);

        // 디버깅 로그
        UE_LOG(LogTemp, Log, TEXT("GroundSpeed: %f, bHasAcceleration: %d"), GroundSpeed, bHasAcceleration);
    }
}
