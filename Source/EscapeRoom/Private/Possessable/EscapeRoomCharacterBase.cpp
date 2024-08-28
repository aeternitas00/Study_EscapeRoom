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

void AEscapeRoomCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TArray<UEscapeRoomIABindableComponent*> BindableComponents;
	GetComponents<UEscapeRoomIABindableComponent>(BindableComponents);

	if (APlayerController* PlayerController = Cast<APlayerController>(NewController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			for(auto& BindComp : BindableComponents)
				BindComp->AddMappingContext(Subsystem);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(NewController->InputComponent))
	{
		for(auto& BindComp : BindableComponents)
			BindComp->BindActions(EnhancedInputComponent);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void AEscapeRoomCharacterBase::UnPossessed()
{	
	// Clear context before engine nullify Controller ref
	TArray<UEscapeRoomIABindableComponent*> BindableComponents;
	GetComponents<UEscapeRoomIABindableComponent>(BindableComponents);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			for(auto& BindComp : BindableComponents)
				BindComp->RemoveMappingContext(Subsystem);
		}
	}

	// This will nullify controller ref
	Super::UnPossessed();
}

void AEscapeRoomCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); 

	// Param "PlayerInputComponent" is PawnInputComponent.
	// We'll use PlayerController's InputComponent; IAs binded to PawnInputComponent won't working if any IA bound to PC_InputComponent.
}

