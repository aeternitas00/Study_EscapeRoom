// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EscapeRoom.h"
#include "GameFramework/PlayerController.h"
#include "Input/EscapeRoomIABindableComponent.h"
#include "EscapeRoomPlayerController.generated.h"

class UEscapeRoomInteractionComponent;
class UInputMappingContext;
class UInputAction;

struct FInputActionInstance;
struct FInputActionValue;



/**
 * 
 */
UCLASS(Blueprintable)
class ESCAPEROOM_API AEscapeRoomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<const UInputMappingContext> CommonMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TMap<FGameplayTag, TObjectPtr<const UInputAction>> GeneralInputActions;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	//TMap<FGameplayTag, FEscapeRoomInputAction> AbilityInputActions;


public:
	AEscapeRoomPlayerController();

protected:
	virtual void BeginPlay() override;	
	virtual void OnPossess(APawn* aPawn) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void JumpEnd(const FInputActionValue& Value);

	// 이런 태그드 인풋 처리하는 커스텀 인풋 컴포넌트 구현?
	//void SendTaggedInputToPawn(const FInputActionInstance& Instance, const FGameplayTag& InputTag);

};
