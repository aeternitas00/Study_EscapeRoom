#include "Yona.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/SpringArmComponent.h"  

AYona::AYona()
{

}

void AYona::BeginPlay()
{
    Super::BeginPlay();

    FVector Location = GetActorLocation() + FVector(200.0f, 0.0f, 0.0f); 
    FRotator Rotation = GetActorRotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
    if (!BatPetClass) return;
	MyPet = GetWorld()->SpawnActor<ABatPet>(BatPetClass, Location, Rotation, SpawnParams);

    if (MyPet)
    {
        MyPet->PlayerCharacter = this;
    }
}


void AYona::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("MoveDirectButton", IE_Pressed, this, &AYona::MovePetDirect);
    PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &AYona::SwitchWithPet);
}

void AYona::MovePetDirect()
{
    if (!MyPet)
    {
        UE_LOG(LogTemp, Warning, TEXT("MyPet is not set."));
        return;
    }

    APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
    if (!CameraManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("CameraManager is not valid."));
        return;
    }

    FVector Start = CameraManager->GetCameraLocation();
    FVector ForwardVector = CameraManager->GetCameraRotation().Vector();
    float TargetDistance = MyPet->BatRange_Max;

    USpringArmComponent* LocalCameraBoom = GetCameraBoom();
    if (LocalCameraBoom)
    {
        TargetDistance += LocalCameraBoom->TargetArmLength;
    }

    FVector End = Start + ForwardVector * TargetDistance;

    FHitResult Hit;
    if (UKismetSystemLibrary::LineTraceSingle(this, Start, End, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::None, Hit, true))
    {
        FVector TargetLocation = Hit.bBlockingHit ? Hit.ImpactPoint : End;
        MyPet->MoveTowardsDirection(TargetLocation);
    }
}





void AYona::SwitchWithPet()
{
    if (!MyPet) return;

    MyPet->SwapWithPlayer();
}
