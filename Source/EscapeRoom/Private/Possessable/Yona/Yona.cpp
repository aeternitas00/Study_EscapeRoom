#include "Possessable/Yona/Yona.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"  
#include "AIController.h"

AYona::AYona()
{
    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    YonaAbilityComponent = CreateDefaultSubobject<UEscapeRoomYonaAbility>(TEXT("YonaAbility"));
    YonaAbilityComponent->OwnerYona = this;
}

void AYona::BeginPlay()
{
    Super::BeginPlay();

    FVector Location = GetActorLocation() + FVector(200.0f, 0.0f, 0.0f); 
    FRotator Rotation = GetActorRotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
    SpawnParams.Owner = this;
    
    if (!BatPetClass) return;

	MyPet = GetWorld()->SpawnActor<ABatPet>(BatPetClass, Location, Rotation, SpawnParams);

    if (MyPet) MyPet->SetOwnerCharacter(this);
    
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
    else 
        MyPet->MoveTowardsDirection(End);
    
}

void AYona::SwitchWithPet()
{
    if (!MyPet) return;

    MyPet->SwapWithPlayer();
}

UEscapeRoomYonaAbility::UEscapeRoomYonaAbility()
{
}

void UEscapeRoomYonaAbility::MainAbility_Implementation(const FInputActionInstance& Instance)
{

}

void UEscapeRoomYonaAbility::SubAbility1_Implementation(const FInputActionInstance& Instance)
{
    OwnerYona->MovePetDirect();
}

void UEscapeRoomYonaAbility::SubAbility2_Implementation(const FInputActionInstance& Instance)
{
    OwnerYona->SwitchWithPet();
}
