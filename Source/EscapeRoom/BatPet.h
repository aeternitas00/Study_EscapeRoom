#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "BatPet.generated.h"

UCLASS()
class ESCAPEROOM_API ABatPet : public APawn
{
    GENERATED_BODY()

public:
    ABatPet();

public:
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
    float BatRange_Max = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RotationSpeed;

    // Target Character
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetCharacter")
    TObjectPtr<AActor> PlayerCharacter;

    // Abilities
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void MoveTowardsDirection(FVector TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SwapWithPlayer();

private:
    FVector TargetLocation;
    bool bIsMovingToTarget;
    bool bIsWaiting;

    void CheckToTarget();
    void HandleMovementAndRotation(float DeltaTime);
    void UpdateCustomDepthOnMovement();
    FVector CalculateDirection(FVector TargetLocationValue);
};
