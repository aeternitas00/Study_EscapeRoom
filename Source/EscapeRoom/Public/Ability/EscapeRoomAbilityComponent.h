// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EscapeRoom.h"
#include "Input/EscapeRoomIABindableComponent.h"
#include "EscapeRoomAbilityComponent.generated.h"

class UInputAction;
class UEnhancedInputComponent;

UCLASS( Blueprintable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UEscapeRoomAbilityComponent : public UEscapeRoomIABindableComponent
{
	GENERATED_BODY()

protected:

public:	
	// Sets default values for this component's properties
	UEscapeRoomAbilityComponent();

protected:	
	virtual void SendTaggedInputToOwner_Implementation(const FInputActionInstance& Instance, const FGameplayTag& InputTag);

public:	
	UFUNCTION(BlueprintImplementableEvent)
	void MainAbility(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintImplementableEvent)
	void SubAbility1(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintImplementableEvent)
	void SubAbility2(const FInputActionInstance& Instance);
};

