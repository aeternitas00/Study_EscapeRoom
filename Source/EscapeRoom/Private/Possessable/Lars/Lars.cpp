#include "Possessable/Lars/Lars.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"  

// Sets default values
ALars::ALars()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LarsAbilityComponent = CreateDefaultSubobject<UEscapeRoomLarsAbility>(TEXT("LarsAbility"));
	LarsAbilityComponent->OwnerLars = this;
}

// Called when the game starts or when spawned
void ALars::BeginPlay()
{
	Super::BeginPlay();
	
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

