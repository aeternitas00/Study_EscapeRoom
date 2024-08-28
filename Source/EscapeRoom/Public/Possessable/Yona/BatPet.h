#pragma once

#include "EscapeRoom.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BatPet.generated.h"

UENUM(BlueprintType)
enum class EBatPetState : uint8
{
    Idle,
    MovingToTarget,
    Waiting,
    Teleported,
};

UCLASS()
class ESCAPEROOM_API ABatPet : public APawn
{
    GENERATED_BODY()

public:
    ABatPet();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* SphereCollider;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* SkeletalMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UFloatingPawnMovement* MovementComponent;

    // Movement
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BatRange_Min;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BatRange_Max;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RotationSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
    EBatPetState CurrentState = EBatPetState::Idle;

    // Target Character
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetCharacter")
    ACharacter* PlayerCharacter;

    FVector TargetLocation;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void MoveTowardsDirection(FVector NewTargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SwapWithPlayer();

    void LookAtTarget(FVector TargetLocation, float DeltaTime);
    void MoveAndLookAtTarget(float DeltaTime, FVector NewTargetLocation);
    FVector CalculateDirection(FVector TargetLocationValue);

    void UpdateCustomDepthOnMovement();
    void CheckToTarget();
    void SetOwnerCharacter(ACharacter* OwnerCharacter);

};
