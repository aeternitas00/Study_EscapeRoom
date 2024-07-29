// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/EscapeRoomAbilityComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EscapeRoomTags.h"
#include "GameFrameWork/Character.h"

// Sets default values for this component's properties
UEscapeRoomAbilityComponent::UEscapeRoomAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEscapeRoomAbilityComponent::SendTaggedInputToOwner_Implementation(const FInputActionInstance& Instance, const FGameplayTag& InputTag)
{
	if(InputTag == EscapeRoomTags::InputTag_MainAbility)
		MainAbility(Instance);	

	else if(InputTag == EscapeRoomTags::InputTag_SubAbility1)
		SubAbility1(Instance);

	else if(InputTag == EscapeRoomTags::InputTag_SubAbility2)
		SubAbility2(Instance);
}

