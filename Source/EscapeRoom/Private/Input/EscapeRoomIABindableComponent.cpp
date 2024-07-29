// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/EscapeRoomIABindableComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

void UEscapeRoomIABindableComponent::BindActions(UEnhancedInputComponent* InputComponent)
{	
	for(auto& TagAbilityPair : DynamicInputActions)
	{
		auto& CurrentIT = TagAbilityPair.Key;
		auto& CurrentIA = TagAbilityPair.Value;

		for(auto TE : CurrentIA.BindingEvents)
			InputComponent->BindActionInstanceLambda(CurrentIA.InputAction, TE, 
				[=,this](const FInputActionInstance& AI,const FGameplayTag& IT){SendTaggedInputToOwner(AI,IT);},
			CurrentIT);
	}
	BindNativeActions(InputComponent);
}

void UEscapeRoomIABindableComponent::BindNativeActions(UEnhancedInputComponent* InputComponent)
{	
	// Example
	//if(!NativeInputActions.Find(EscapeRoomTags::InputTag_Interact)) return;

	//InputComponent->BindAction(NativeInputActions[EscapeRoomTags::InputTag_Interact], ETriggerEvent::Started, this, &ThisClass::Interact);
	//InputComponent->BindAction(NativeInputActions[EscapeRoomTags::InputTag_Interact], ETriggerEvent::Completed, this, &ThisClass::InteractEnd);
}

void UEscapeRoomIABindableComponent::SendTaggedInputToOwner_Implementation(const FInputActionInstance& Instance, const FGameplayTag& InputTag)
{
	// Example

	//auto LocalPawn = Cast<AEscapeRoomCharacterBase>(GetOwner());
	//if (!LocalPawn) return;

	//if(InputTag == EscapeRoomTags::InputTag_MainAbility)
	//	LocalPawn->MainAbility(Instance);	

	//else if(InputTag == EscapeRoomTags::InputTag_SubAbility1)
	//	LocalPawn->SubAbility1(Instance);

	//else if(InputTag == EscapeRoomTags::InputTag_SubAbility2)
	//	LocalPawn->SubAbility2(Instance);
}
