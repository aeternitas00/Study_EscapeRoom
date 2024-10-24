// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EscapeRoomPlayerController.h"

#include "Possessable/EscapeRoomCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "Interactable/EscapeRoomInteractionComponent.h"
#include "Input/EscapeRoomIABindableComponent.h"

#include "EscapeRoomTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


AEscapeRoomPlayerController::AEscapeRoomPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = true;  
	//InteractionComponent = CreateDefaultSubobject<UEscapeRoomInteractionComponent>("InteractionComponent");
}

void AEscapeRoomPlayerController::BeginPlay()
{
    Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(CommonMappingContext, 0);
	}

	// Set up action bindings

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{	
		// Natives
		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Move], ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Look], ETriggerEvent::Triggered, this, &ThisClass::Look);

		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Jump], ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Jump], ETriggerEvent::Completed, this, &ThisClass::JumpEnd);
		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Jump], ETriggerEvent::Triggered, this, &ThisClass::FlyUp);

		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Crouch], ETriggerEvent::Started, this, &ThisClass::Crouch);
		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Crouch], ETriggerEvent::Completed, this, &ThisClass::UnCrouch);
		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Crouch], ETriggerEvent::Triggered, this, &ThisClass::FlyDown);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AEscapeRoomPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AEscapeRoomPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	auto LocalPawn = GetPawn();
	if (!LocalPawn) return;

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// TEMP
	bool MovementFlag = !LocalPawn->GetComponentByClass<UFloatingPawnMovement>();

	if (MovementFlag)
	{
		LocalPawn->AddMovementInput( FRotationMatrix(YawRotation).GetUnitAxis( EAxis::X ), MovementVector.Y);
		LocalPawn->AddMovementInput( FRotationMatrix(YawRotation).GetUnitAxis( EAxis::Y ), MovementVector.X);
	}
	else 
	{
		LocalPawn->AddMovementInput( FRotationMatrix(Rotation).GetScaledAxis( EAxis::X ), MovementVector.Y );
		LocalPawn->AddMovementInput( FRotationMatrix(Rotation).GetScaledAxis( EAxis::Y ), MovementVector.X );
	}
}

void AEscapeRoomPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	auto LocalPawn = GetPawn();
	if (!LocalPawn) return;

	// add yaw and pitch input to controller
	LocalPawn->AddControllerYawInput(LookAxisVector.X);
	LocalPawn->AddControllerPitchInput(LookAxisVector.Y);
}

void AEscapeRoomPlayerController::Jump(const FInputActionValue& Value)
{
	auto LocalPawn = Cast<ACharacter>(GetPawn());
	if (!LocalPawn) return;
	LocalPawn->Jump();
}

void AEscapeRoomPlayerController::JumpEnd(const FInputActionValue& Value)
{
	auto LocalPawn = Cast<ACharacter>(GetPawn());
	if (!LocalPawn) return;
	LocalPawn->StopJumping();
}

void AEscapeRoomPlayerController::Crouch(const FInputActionValue& Value)
{
	auto LocalPawn = Cast<ACharacter>(GetPawn());
	if (!LocalPawn) return;
	LocalPawn->Crouch();
}

void AEscapeRoomPlayerController::UnCrouch(const FInputActionValue& Value)
{
	auto LocalPawn = Cast<ACharacter>(GetPawn());
	if (!LocalPawn) return;
	LocalPawn->UnCrouch();
}

void AEscapeRoomPlayerController::FlyUp(const FInputActionValue& Value)
{
	auto LocalPawn = Cast<ACharacter>(GetPawn());
	if (!LocalPawn) return;

	// TEMP
	if(!!LocalPawn->GetComponentByClass<UFloatingPawnMovement>())
	{
		UE_LOG(LogTemplateCharacter,Display,TEXT("FLOATING UP"));
		LocalPawn->AddMovementInput(FVector::UpVector, Value.GetMagnitude());
	}
}

void AEscapeRoomPlayerController::FlyDown(const FInputActionValue& Value)
{
	auto LocalPawn = Cast<ACharacter>(GetPawn());
	if (!LocalPawn) return;
	// TEMP
	if(!!LocalPawn->GetComponentByClass<UFloatingPawnMovement>())
	{
		UE_LOG(LogTemplateCharacter,Display,TEXT("FLOATING DOWN"));
		LocalPawn->AddMovementInput(FVector::DownVector, Value.GetMagnitude());
	}
}