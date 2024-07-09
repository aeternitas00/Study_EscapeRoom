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

    // �ʱ� ���� ����
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

    // �÷��̾� üũ
    if (!PlayerCharacter) {
        return;
    }

    // �÷��̾� ��ġ / ���� ��ġ / ���� / �Ÿ�
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector BatLocation = GetActorLocation();
    FVector DirectionToPlayer = (PlayerLocation - BatLocation).GetSafeNormal();
    float DistanceToPlayer = FVector::Dist(BatLocation, PlayerLocation);

    // �÷��̾� ����ٴϱ� ����
    // Ÿ�� �̵����� �ʰ� ��� �ƴ� ����
    if (!bIsMovingToTarget && !bIsWaiting) {
        // �� �Ÿ��� ���� �������� ������
        if (DistanceToPlayer > BatRange_Idle) {
            //Ÿ�� ��ġ = �÷��̾� ��ġ
            TargetLocation = PlayerLocation;
            MoveBat(DeltaTime, DirectionToPlayer);
        }
    }
    // Ÿ������ �̵��ϴ� ����
    else if (bIsMovingToTarget) {
        MoveBat(DeltaTime, DirectionToPlayer);
    }

    //�׻� ���� ����
    HandleRotation(DeltaTime, DirectionToPlayer);
}



//���� �ൿ ���� ����
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

//���� ���� ����
void ABatPet::HandleRotation(float DeltaTime, const FVector& DirectionToPlayer)
{
    FRotator CurrentRotation = GetActorRotation();
    FRotator TargetRotation = DirectionToPlayer.Rotation() + FRotator(0, 90, 0);
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
    SetActorRotation(NewRotation);
}

//���� �÷��̾� ���� �̵�
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
        bIsMovingToTarget = false;  // �̵� ����
    }
    else
    {
        SetActorLocation(NewLocation);
        if (FVector::Dist(NewLocation, TargetLocation) <= 100.0f)  // ��ǥ ��ġ�� �����ϸ�
        {
            bIsMovingToTarget = false;
            bIsWaiting = true;  // ��� ���·� ��ȯ
        }
    }
}

// ������ġ�� �̵��ϴ� �Լ�
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

//�÷��̾�� ��ġ ����
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
