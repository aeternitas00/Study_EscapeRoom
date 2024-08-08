#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "BatPet.generated.h"

UENUM(BlueprintType)
enum class EBatPetState : uint8
{
    Idle,
    MovingToTarget,
    Waiting
};

UCLASS()
class ESCAPEROOM_API ABatPet : public APawn
{
    GENERATED_BODY()

public:
    ABatPet();

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


    // Target Character
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetCharacter")
    ACharacter* PlayerCharacter;

    FVector TargetLocation;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void MoveTowardsDirection(FVector NewTargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SwapWithPlayer();

    void LookAtTarget(FVector TargetLocation, float DeltaTime);
    void MoveAndLookAtTarget(float DeltaTime, FVector NewTargetLocation);
    FVector CalculateDirection(FVector TargetLocationValue);

    void UpdateCustomDepthOnMovement();
    void CheckToTarget();

private:
    EBatPetState CurrentState = EBatPetState::Idle;
};
