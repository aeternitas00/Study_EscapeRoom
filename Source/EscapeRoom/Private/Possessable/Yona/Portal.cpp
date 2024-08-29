#include "Possessable/Yona/Portal.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"

#include "Possessable/Yona/BatPet.h"

APortal::APortal()
{
    PrimaryActorTick.bCanEverTick = true;

    PortalEntrance = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalEntrance"));
    RootComponent = PortalEntrance;
    PortalEntrance->SetCollisionProfileName(TEXT("Trigger"));
    PortalEntrance->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);

    ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
    ArrowComponent->SetupAttachment(RootComponent);

    TargetLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TargetLocation"));
    TargetLocation->SetupAttachment(RootComponent);

    AllowedClass = AActor::StaticClass();
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor != nullptr && OtherActor != this && OtherActor->IsA(AllowedClass))
    {
        if (TargetPortal && TargetPortal->TargetLocation)
        {
            FVector TargetLocationPosition = TargetPortal->TargetLocation->GetComponentLocation();
            FRotator TargetLocationRotation = TargetPortal->TargetLocation->GetComponentRotation();

            OtherActor->SetActorLocationAndRotation(TargetLocationPosition, TargetLocationRotation, false, nullptr, ETeleportType::TeleportPhysics);
            Cast<ABatPet>(OtherActor)->CurrentState = EBatPetState::Teleported;

        }
    }
}