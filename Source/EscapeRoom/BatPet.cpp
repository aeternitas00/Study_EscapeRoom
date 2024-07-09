#include "BatPet.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"

ABatPet::ABatPet()
{
    PrimaryActorTick.bCanEverTick = true;

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SetRootComponent(SkeletalMeshComponent);

    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(RootComponent);
    SphereCollider->SetSphereRadius(50.0f);
    SphereCollider->SetCollisionProfileName(TEXT("Pawn"));

    // 초기 변수 세팅
    BatRange_Idle = 300.0f;
    BatRange_Move = 1000.0f;
    MovementSpeed = 200.0f;
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

    // 플레이어 체크
    if (!PlayerCharacter) {
        return;
    }

    // 플레이어 위치 / 박쥐 위치 / 방향 / 거리
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector BatLocation = GetActorLocation();
    FVector DirectionToPlayer = (PlayerLocation - BatLocation).GetSafeNormal();
    float DistanceToPlayer = FVector::Dist(BatLocation, PlayerLocation);

    // 플레이어 따라다니기 로직
    // 타겟 이동하지 않고 대기 아닌 상태
    if (!bIsMovingToTarget && !bIsWaiting) {
        // 두 거리가 박쥐 범위보다 넓으면
        if (DistanceToPlayer > BatRange_Idle) {
            //타겟 위치 = 플레이어 위치
            TargetLocation = PlayerLocation;
            MoveBat(DeltaTime, DirectionToPlayer);
        }
    }
    // 타겟한테 이동하는 상태
    else if (bIsMovingToTarget) {
        MoveBat(DeltaTime, DirectionToPlayer);
    }

    //항상 각도 조정
    HandleRotation(DeltaTime, DirectionToPlayer);
}



//박쥐 행동 조건 업뎃
void ABatPet::UpdateBatMovement(float DeltaTime, const FVector& PlayerLocation, const FVector& BatLocation, const FVector& DirectionToPlayer, float DistanceToPlayer)
{
    if (DistanceToPlayer > BatRange_Idle) {
        TargetLocation = PlayerLocation - DirectionToPlayer * BatRange_Idle;
    }
    else if (DistanceToPlayer < 100.0f) {
        TargetLocation = BatLocation - DirectionToPlayer * BatRange_Idle;
    }
    MoveBat(DeltaTime, DirectionToPlayer);
}

//박쥐 각도 조정
void ABatPet::HandleRotation(float DeltaTime, const FVector& DirectionToPlayer)
{
    FRotator CurrentRotation = GetActorRotation();
    FRotator TargetRotation = DirectionToPlayer.Rotation() + FRotator(0, 90, 0);
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
    SetActorRotation(NewRotation);
}

//박쥐 플레이어 따라서 이동
void ABatPet::MoveBat(float DeltaTime, const FVector& DirectionToPlayer)
{
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->SweepSingleByChannel(HitResult, CurrentLocation, NewLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SphereCollider->GetScaledSphereRadius()), Params))
    {
        bIsMovingToTarget = false;  // 이동 중지
    }
    else
    {
        SetActorLocation(NewLocation);
        if (FVector::Dist(NewLocation, TargetLocation) <= 100.0f)  // 목표 위치에 근접하면
        {
            bIsMovingToTarget = false;
            bIsWaiting = true;  // 대기 상태로 전환
        }
    }
}

// 지정위치로 이동하는 함수
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

//플레이어와 위치 스왑
void ABatPet::SwapWithPlayer()
{
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
