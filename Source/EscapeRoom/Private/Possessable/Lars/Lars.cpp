#include "Possessable/Lars/Lars.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"  

// Sets default values
ALars::ALars()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LarsAbilityComponent = CreateDefaultSubobject<UEscapeRoomLarsAbility>(TEXT("LarsAbility"));
	LarsAbilityComponent->OwnerLars = this;
    bIsMagnetActive = false; 
}

// Called when the game starts or when spawned
void ALars::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALars::ToggleMagnetism()
{
    if (bIsMagnetActive && TargetedObject) {
        // 이미 활성화된 경우 물체 해제
        UPrimitiveComponent* HitComponent = Cast<UPrimitiveComponent>(TargetedObject->GetComponentByClass(UPrimitiveComponent::StaticClass()));
        if (HitComponent) {
            HitComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 다시 활성화
            HitComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        }
        TargetedObject = nullptr;
        bIsMagnetActive = false;
        UE_LOG(LogTemp, Log, TEXT("Magnetism deactivated"));
    }
    else {
        // Magnet 활성화 시도
        if (TargetedObject) {
            // 이미 물체를 들고 있는 상태에서는 새 물체를 선택하지 않음
            return;
        }

        FVector StartPoint = GetActorLocation();
        FVector ForwardVector = GetActorForwardVector();
        FVector EndPoint = StartPoint + ForwardVector * 1000;
        FHitResult Hit;
        FCollisionQueryParams CollisionParams;

        bool bHit = GetWorld()->LineTraceSingleByObjectType(Hit, StartPoint, EndPoint, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), CollisionParams);
        DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Green, false, 1.0f, 0, 1.0f);

        if (bHit && Hit.GetActor()) {
            UPrimitiveComponent* HitComponent = Cast<UPrimitiveComponent>(Hit.GetComponent());
            if (HitComponent) {
                // 물리 시뮬레이션 유지
                HitComponent->SetSimulatePhysics(true);
                HitComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
                TargetedObject = Hit.GetActor();
                bIsMagnetActive = true;
                UE_LOG(LogTemp, Log, TEXT("Magnetism activated: %s"), *TargetedObject->GetName());
            }
        }
    }
}

void ALars::ApplyForceBasedOnCharge(AActor* Actor, FVector ImpactPoint)
{
    if (!bIsMagnetActive || !TargetedObject) {
        UE_LOG(LogTemp, Warning, TEXT("No object selected or magnetism is not active."));
        return;
    }

    bool isObjectPositive = TargetedObject->Tags.Contains("Positive");
    bool isObjectNegative = TargetedObject->Tags.Contains("Negative");
    FVector PlayerPosition = GetActorLocation();
    FVector ObjectPosition = TargetedObject->GetActorLocation();
    FVector DirectionToPlayer = (PlayerPosition - ObjectPosition).GetSafeNormal();
    FVector ForceDirection;

    if ((CurrentChargeState == EChargeState::Positive && isObjectNegative) ||
        (CurrentChargeState == EChargeState::Negative && isObjectPositive)) {
        ForceDirection = DirectionToPlayer;
    }
    else {
        ForceDirection = -DirectionToPlayer;
    }

    UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(TargetedObject->GetComponentByClass(UPrimitiveComponent::StaticClass()));
    if (RootComp) {
        RootComp->AddForce(ForceDirection * 5000, NAME_None, true);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("No valid PrimitiveComponent found on the target object."));
    }
}

void ALars::ToggleChargeState()
{
    CurrentChargeState = static_cast<EChargeState>((static_cast<int>(CurrentChargeState) + 1) % 2);
    FString ChargeStateString = (CurrentChargeState == EChargeState::Positive) ? TEXT("Positive") : TEXT("Negative");
    UE_LOG(LogTemp, Log, TEXT("Charge state changed to %s"), *ChargeStateString);
}


UEscapeRoomLarsAbility::UEscapeRoomLarsAbility()
{
}

void UEscapeRoomLarsAbility::MainAbility_Implementation(const FInputActionInstance& Instance)
{

}

void UEscapeRoomLarsAbility::SubAbility1_Implementation(const FInputActionInstance& Instance)
{
}

void UEscapeRoomLarsAbility::SubAbility2_Implementation(const FInputActionInstance& Instance)
{
}

