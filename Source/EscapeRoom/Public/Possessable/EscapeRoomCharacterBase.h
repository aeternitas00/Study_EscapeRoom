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
	// APawn interface
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	//virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void MainAbility(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintImplementableEvent)
	void SubAbility1(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintImplementableEvent)
	void SubAbility2(const FInputActionInstance& Instance);
};

