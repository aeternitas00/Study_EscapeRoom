// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EscapeRoom.h"
#include "GameFramework/Character.h"
#include "EscapeRoomCharacterBase.generated.h"

class UInputAction;
class UEscapeRoomAbility;
class UEscapeRoomActiveAbility;

struct FInputActionInstance;

UCLASS(config=Game)
class AEscapeRoomCharacterBase : public ACharacter
{
	GENERATED_BODY()
	
protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	//TMap<TObjectPtr<UEscapeRoomAbility>, TObjectPtr<const UInputAction>> AbilityInputActions;

	// Or as active ability handle
	//TMap<TObjectPtr<UEscapeRoomAbility>, TObjectPtr<UEscapeRoomAbility>> ActiveAbilities;
public:
	AEscapeRoomCharacterBase();
	
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();
public:

};

