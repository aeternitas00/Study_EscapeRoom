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

//���� �ൿ ���� ����
void ABatPet::UpdateMovementAndRotation(float DeltaTime)
{
    // �÷��̾� ��ġ / ���� ��ġ / ���� / �Ÿ�
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector BatLocation = GetActorLocation();
    FVector DirectionToPlayer = (PlayerLocation - BatLocation).GetSafeNormal();
    float DistanceToPlayer = FVector::Dist(BatLocation, PlayerLocation);

    // �÷��̾�� �Ÿ��� �־����� �̵�
    if (!bIsMovingToTarget && !bIsWaiting && DistanceToPlayer > BatRange_Idle) {
        TargetLocation = PlayerLocation;
        MoveBat(DeltaTime, DirectionToPlayer);
    }
    // Ÿ���� ������ ���
    else if (bIsMovingToTarget) {
        MoveBat(DeltaTime, DirectionToPlayer);
    }

    //�÷��̾� �ٶ󺸴� ����
    FRotator CurrentRotation = GetActorRotation();
    FRotator TargetRotation = DirectionToPlayer.Rotation() + FRotator(0, 90, 0);
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

    SetActorRotation(NewRotation);
}

//���� �̵� �Լ�
void ABatPet::MoveBat(float DeltaTime, const FVector& DirectionToPlayer)
{
    // ���� ��ġ�� ����
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;

    // �� �浹 ����
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    // �浹 �� ����
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

// ������ġ�� �̵��ϴ� �Լ� Q
void ABatPet::MoveTowardsDirection(FVector NewTargetLocation)
{
    if (bIsWaiting) {
        // ��� ���¿��� Q�� �ٽ� ������ �÷��̾ ����ٴϴ� ���·� ��ȯ
        bIsWaiting = false;
        bIsMovingToTarget = false; // �̵� ����, �÷��̾� ����ٴϱ� �簳
        UE_LOG(LogTemp, Warning, TEXT("Resuming following the player."));
    }
    else {
        // ó�� Q�� ������ Ÿ�� �������� �̵�
        TargetLocation = NewTargetLocation;
        bIsMovingToTarget = true;
        bIsWaiting = true; // ��� ���� Ȱ��ȭ
        UE_LOG(LogTemp, Warning, TEXT("Moving to new target location: %s"), *TargetLocation.ToString());
    }
}

//�÷��̾�� ��ġ ���� E
void ABatPet::SwapWithPlayer()
{
    // �÷��̾�� ���� ��ġ ����
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
