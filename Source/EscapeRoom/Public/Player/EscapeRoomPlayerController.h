// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EscapeRoom.h"
#include "GameFramework/PlayerController.h"
#include "InputTriggers.h"
#include "EscapeRoomPlayerController.generated.h"

class UEscapeRoomInteractionComponent;
class UInputMappingContext;
class UInputAction;

struct FInputActionInstance;
struct FInputActionValue;


USTRUCT(BlueprintType)
struct ESCAPEROOM_API FEscapeRoomInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<const UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TSet<ETriggerEvent> BindingEvents;
};

/**
 * 
 */
UCLASS(Blueprintable)
class ESCAPEROOM_API AEscapeRoomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	//TObjectPtr<UEscapeRoomInteractionComponent> InteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<const UInputMappingContext> CharacterMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TMap<FGameplayTag, TObjectPtr<const UInputAction>> GeneralInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TMap<FGameplayTag, FEscapeRoomInputAction> AbilityInputActions;


public:
	AEscapeRoomPlayerController();

protected:
	virtual void BeginPlay() override;	
	virtual void OnPossess(APawn* aPawn) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void JumpEnd(const FInputActionValue& Value);

	// �̷� �±׵� ��ǲ ó���ϴ� Ŀ���� ��ǲ ������Ʈ ����?
	void SendTaggedInputToPawn(const FInputActionInstance& Instance, const FGameplayTag& InputTag);

};
