// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EscapeRoomPlayerController.h"

#include "Possessable/EscapeRoomCharacterBase.h"
#include "GameFramework/Character.h"

#include "Interactable/EscapeRoomInteractionComponent.h"

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
		Subsystem->AddMappingContext(CharacterMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{	
		// There must be a better way... 

		// Natives
		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Move], ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Look], ETriggerEvent::Triggered, this, &ThisClass::Look);

		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Jump], ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(GeneralInputActions[EscapeRoomTags::InputTag_Jump], ETriggerEvent::Completed, this, &ThisClass::JumpEnd);

		// 인풋을 가질 수 있는 컴포넌트로 일괄 처리?
		auto IntComp = GetComponentByClass<UEscapeRoomInteractionComponent>(); 
		if (IntComp) IntComp->BindActions(EnhancedInputComponent);

		// Tagged actions
		// 이것도 컴포넌트로 ?
		for(auto& TagAbilityPair : AbilityInputActions)
		{
			auto& CurrentIT = TagAbilityPair.Key;
			auto& CurrentIA = TagAbilityPair.Value;

			for(auto TE : CurrentIA.BindingEvents)
				EnhancedInputComponent->BindActionInstanceLambda(CurrentIA.InputAction, TE, 
					[=,this](const FInputActionInstance& AI,const FGameplayTag& IT){SendTaggedInputToPawn(AI,IT);},
				CurrentIT);
		}
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

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	LocalPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
	LocalPawn->AddMovementInput(RightDirection, MovementVector.X);
	
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

void AEscapeRoomPlayerController::SendTaggedInputToPawn(const FInputActionInstance& Instance, const FGameplayTag& InputTag)
{
	auto LocalPawn = Cast<AEscapeRoomCharacterBase>(GetPawn());
	if (!LocalPawn) return;

	// 이것도 태그-어빌리티 컨테이너로 구현 할 수 있다면...... 바인드 시점을 BeginPlay로 두거나 뭐 어쩌든지 해서...
	if(InputTag == EscapeRoomTags::InputTag_MainAbility)
		LocalPawn->MainAbility(Instance);	
	
	else if(InputTag == EscapeRoomTags::InputTag_SubAbility1)
		LocalPawn->SubAbility1(Instance);
	
	else if(InputTag == EscapeRoomTags::InputTag_SubAbility2)
		LocalPawn->SubAbility2(Instance);
	
}
