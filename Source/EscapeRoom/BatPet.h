#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "EscapeRoomCharacter.h"
#include "BatPet.generated.h"

UCLASS()
class ESCAPEROOM_API ABatPet : public APawn
{
    GENERATED_BODY()

public:
    ABatPet();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    //모델
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* SkeletalMeshComponent;

    //충돌을 위한 콜리전
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* SphereCollider;

    //Bat Status
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BatRange_Idle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BatRange_Move;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RotationSpeed;

    //Target Player
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    AEscapeRoomCharacter* PlayerCharacter;

    //Bat 능력 함수
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void MoveTowardsDirection(FVector TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SwapWithPlayer();

private:
    FVector TargetLocation;
    bool bIsMovingToTarget;
    bool bIsWaiting;
    FVector LastPlayerLocation;

    void MoveBat(float DeltaTime, const FVector& DirectionToPlayer);
    void UpdateBatMovement(float DeltaTime, const FVector& PlayerLocation, const FVector& BatLocation, const FVector& DirectionToPlayer, float DistanceToPlayer);
    void HandleRotation(float DeltaTime, const FVector& DirectionToPlayer);
};
