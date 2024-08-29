#pragma once

#include "EscapeRoom.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class ESCAPEROOM_API APortal : public AActor
{
    GENERATED_BODY()

public:
    APortal();

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UBoxComponent* PortalEntrance;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UArrowComponent* ArrowComponent;

    UPROPERTY(EditAnywhere, Category = "Portal")
    APortal* TargetPortal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
    TSubclassOf<AActor> AllowedClass;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USceneComponent* TargetLocation;

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
