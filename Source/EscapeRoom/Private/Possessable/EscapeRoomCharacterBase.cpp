// Copyright Epic Games, Inc. All Rights Reserved.

#include "Possessable/EscapeRoomCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Interactable/EscapeRoomInteractionComponent.h"


AEscapeRoomCharacterBase::AEscapeRoomCharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

void AEscapeRoomCharacterBase::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// 액션 바인더블한 컴포넌트의 베이스를 만들어서 일괄처리?
	
}

//////////////////////////////////////////////////////////////////////////
// Input

//void AEscapeRoomCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
//		auto IntComp = GetComponentByClass<UEscapeRoomInteractionComponent>(); 
//		if (IntComp) IntComp->BindActions(EnhancedInputComponent);
//	}
//}

