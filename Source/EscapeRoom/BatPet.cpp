#include "BatPet.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

ABatPet::ABatPet()
{
    PrimaryActorTick.bCanEverTick = true;

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SetRootComponent(SkeletalMeshComponent);

    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(RootComponent);
    SphereCollider->SetSphereRadius(50.0f);
    SphereCollider->SetCollisionProfileName(TEXT("Pawn"));

    BatRange_Idle = 300.0f;
    BatRange_Move = 1000.0f;
    MovementSpeed = 800.0f;
    RotationSpeed = 2.0f;
    bIsMovingToTarget = false;
    bIsWaiting = false;
}

void ABatPet::BeginPlay()
{
    Super::BeginPlay();

    if (PlayerCharacter)
    {
        LastPlayerLocation = PlayerCharacter->GetActorLocation();
    }
}

void ABatPet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!PlayerCharacter) return;
    UpdateMovementAndRotation(DeltaTime);
}

//박쥐 행동 조건 업뎃
void ABatPet::UpdateMovementAndRotation(float DeltaTime)
{
    // 플레이어 위치 / 박쥐 위치 / 방향 / 거리
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector BatLocation = GetActorLocation();
    FVector DirectionToPlayer = (PlayerLocation - BatLocation).GetSafeNormal();
    float DistanceToPlayer = FVector::Dist(BatLocation, PlayerLocation);

    // 플레이어와 거리가 멀어지면 이동
    if (!bIsMovingToTarget && !bIsWaiting && DistanceToPlayer > BatRange_Idle) {
        TargetLocation = PlayerLocation;
        MoveBat(DeltaTime, DirectionToPlayer);
    }
    // 타겟이 지정된 경우
    else if (bIsMovingToTarget) {
        MoveBat(DeltaTime, DirectionToPlayer);
    }

    //플레이어 바라보는 각도
    FRotator CurrentRotation = GetActorRotation();
    FRotator TargetRotation = DirectionToPlayer.Rotation() + FRotator(0, 90, 0);
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

    SetActorRotation(NewRotation);
}

//박쥐 이동 함수
void ABatPet::MoveBat(float DeltaTime, const FVector& DirectionToPlayer)
{
    // 현재 위치와 방향
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;

    // 벽 충돌 감지
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    // 충돌 시 멈춤
    if (GetWorld()->SweepSingleByChannel(HitResult, CurrentLocation, NewLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SphereCollider->GetScaledSphereRadius()), Params)) {
        bIsMovingToTarget = false;
    }
    else {
        SetActorLocation(NewLocation);
        if (FVector::Dist(NewLocation, TargetLocation) <= 100.0f) {
            bIsMovingToTarget = false;
            bIsWaiting = true;
        }
    }
}

// 지정위치로 이동하는 함수 Q
void ABatPet::MoveTowardsDirection(FVector NewTargetLocation)
{
    if (bIsWaiting) {
        // 대기 상태에서 Q를 다시 누르면 플레이어를 따라다니는 상태로 전환
        bIsWaiting = false;
        bIsMovingToTarget = false; // 이동 중지, 플레이어 따라다니기 재개
        UE_LOG(LogTemp, Warning, TEXT("Resuming following the player."));
    }
    else {
        // 처음 Q를 누르면 타겟 지점으로 이동
        TargetLocation = NewTargetLocation;
        bIsMovingToTarget = true;
        bIsWaiting = true; // 대기 상태 활성화
        UE_LOG(LogTemp, Warning, TEXT("Moving to new target location: %s"), *TargetLocation.ToString());
    }
}

//플레이어와 위치 스왑 E
void ABatPet::SwapWithPlayer()
{
    // 플레이어랑 박쥐 위치 변경
    if (PlayerCharacter)
    {
        FVector PlayerLocation = PlayerCharacter->GetActorLocation();
        FVector BatLocation = GetActorLocation();

        PlayerCharacter->SetActorLocation(BatLocation);
        SetActorLocation(PlayerLocation);

        bIsMovingToTarget = false;
        UE_LOG(LogTemp, Warning, TEXT("Swap Success"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Swap Fail: PlayerCharacter is not valid"));
    }
}
