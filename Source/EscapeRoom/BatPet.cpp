#include "BatPet.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ABatPet::ABatPet()
{
    PrimaryActorTick.bCanEverTick = true;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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
}


void ABatPet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerCharacter) return;

    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector BatLocation = GetActorLocation();
    float DistanceToPlayer = FVector::Dist(BatLocation, PlayerLocation);

    switch (CurrentState)
    {
    case EBatPetState::Idle:
        if (DistanceToPlayer > BatRange_Min)
        {
            FVector DirectionToPlayer = CalculateDirection(PlayerLocation);
            MovementComponent->AddInputVector(DirectionToPlayer, true);
        }
        LookAtTarget(PlayerLocation, DeltaTime);
        break;
    case EBatPetState::MovingToTarget:
        MoveAndLookAtTarget(DeltaTime, TargetLocation);
        break;
    case EBatPetState::Waiting:
        LookAtTarget(PlayerLocation, DeltaTime);
        break;
    }

    UpdateCustomDepthOnMovement();
}

void ABatPet::CheckToTarget()
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerLocation);

    if (DistanceToPlayer > BatRange_Min)
    {
        CurrentState = EBatPetState::MovingToTarget;
        TargetLocation = PlayerLocation;
    }
}

void ABatPet::MoveAndLookAtTarget(float DeltaTime, FVector NewTargetLocation)
{
    FVector DirectionToTarget = CalculateDirection(NewTargetLocation);
    MovementComponent->AddInputVector(DirectionToTarget, true);
    LookAtTarget(NewTargetLocation, DeltaTime);

    if (FVector::Dist(GetActorLocation(), NewTargetLocation) <= 100.0f) {
        CurrentState = EBatPetState::Waiting;
    }
}

void ABatPet::LookAtTarget(FVector NewTargetLocation, float DeltaTime)
{
    FVector DirectionToTarget = CalculateDirection(NewTargetLocation);
    FRotator TargetRotation = DirectionToTarget.Rotation();
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed));
}



FVector ABatPet::CalculateDirection(FVector TargetLocationValue)
{
    return (TargetLocationValue - GetActorLocation()).GetSafeNormal();
}

// MoveToTarget Ability
void ABatPet::MoveTowardsDirection(FVector NewTargetLocation)
{
    TargetLocation = NewTargetLocation;
    if (CurrentState == EBatPetState::Waiting) {
        CurrentState = EBatPetState::Idle;
    }
    else {
        CurrentState = EBatPetState::MovingToTarget;
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
        CurrentState = EBatPetState::Waiting;
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