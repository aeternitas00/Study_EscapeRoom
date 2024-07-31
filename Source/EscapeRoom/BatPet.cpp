#include "BatPet.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SkeletalMeshComponent.h"

ABatPet::ABatPet()
{
    PrimaryActorTick.bCanEverTick = true;

    // Sphere Collider Settings
    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SetRootComponent(SphereCollider);
    SphereCollider->SetSphereRadius(150.0f);
    SphereCollider->SetCollisionProfileName(TEXT("Pawn"));
    SphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SphereCollider->SetUseCCD(true);

    // Skeletal Mesh Settings
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComponent->SetupAttachment(RootComponent);

    // MovementComponent Settings
    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
    MovementComponent->UpdatedComponent = RootComponent;
    MovementComponent->MaxSpeed = 600.0f;

    bIsMovingToTarget = false;
    bIsWaiting = false;
}

void ABatPet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Player Check
    if (!PlayerCharacter) return;

    CheckToTarget();
    HandleMovementAndRotation(DeltaTime);
    UpdateCustomDepthOnMovement();
}

// CheckTarget
void ABatPet::CheckToTarget()
{
    if (bIsMovingToTarget && FVector::Dist(GetActorLocation(), TargetLocation) <= 100.0f) {
        bIsMovingToTarget = false;
        bIsWaiting = true;
    }
}

// Move and Rotation
void ABatPet::HandleMovementAndRotation(float DeltaTime)
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter is not set."));
        return;
    }

    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector BatLocation = GetActorLocation();
    FVector DirectionToPlayer = CalculateDirection(PlayerLocation);
    float DistanceToPlayer = FVector::Dist(BatLocation, PlayerLocation);

    UE_LOG(LogTemp, Log, TEXT("DistanceToPlayer: %f, BatRange_Min: %f"), DistanceToPlayer, BatRange_Min);

    if (!bIsMovingToTarget && !bIsWaiting)
    {
        if (DistanceToPlayer > BatRange_Min)
        {
            UE_LOG(LogTemp, Log, TEXT("Adding input vector towards player."));
            MovementComponent->AddInputVector(DirectionToPlayer, true);
        }
    }
    else if (bIsMovingToTarget)
    {
        FVector DirectionToTarget = CalculateDirection(TargetLocation);
        MovementComponent->AddInputVector(DirectionToTarget, true);
    }

    FRotator TargetRotation = DirectionToPlayer.Rotation();
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed));
}


FVector ABatPet::CalculateDirection(FVector TargetLocationValue)
{
    return (TargetLocationValue - GetActorLocation()).GetSafeNormal();
}

// Move Ability
void ABatPet::MoveTowardsDirection(FVector NewTargetLocation)
{
    if (bIsWaiting) {
        bIsWaiting = false;
        bIsMovingToTarget = false;
    }
    else {
        TargetLocation = NewTargetLocation;
        bIsMovingToTarget = true;
        bIsWaiting = false;
    }
}

// Swap Ability
void ABatPet::SwapWithPlayer()
{
    if (PlayerCharacter)
    {
        FVector PlayerLocation = PlayerCharacter->GetActorLocation();
        FVector BatLocation = GetActorLocation();
        PlayerCharacter->SetActorLocation(BatLocation);
        SetActorLocation(PlayerLocation);
        bIsMovingToTarget = false;
    }
}

// Outline Func
void ABatPet::UpdateCustomDepthOnMovement()
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector BatLocation = GetActorLocation();
    float DistanceToPlayer = FVector::Dist(BatLocation, PlayerLocation);

    if (DistanceToPlayer < BatRange_Min + 50.0f) {
        SkeletalMeshComponent->SetRenderCustomDepth(false);
    }
    else {
        SkeletalMeshComponent->SetRenderCustomDepth(true);
    }
}
