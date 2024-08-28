// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EscapeRoom.h"
#include "Input/EscapeRoomIABindableComponent.h"
#include "Interactable/EscapeRoomInteractable.h"
#include "EscapeRoomInteractionComponent.generated.h"

class UInputAction;
class UEnhancedInputComponent;

UCLASS( Blueprintable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UEscapeRoomInteractionComponent : public UEscapeRoomIABindableComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interact)
	FGameplayTag InteractionMode;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interact)
	bool InteractionLocked;

	// 두개 분할할 이유는 없는 듯 하기도 하고.. 뭔가 상호 작용 하고 있을 때 인스펙션이 의미 있는 경우가 있을까?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interact)
	TScriptInterface<IEscapeRoomInteractable> InspectingObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interact)
	TScriptInterface<IEscapeRoomInteractable> InteractingObject;

public:	
	// Sets default values for this component's properties
	UEscapeRoomInteractionComponent();

protected:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BindNativeActions(UEnhancedInputComponent* InputComponent) override;

	void GetTracePoints(FVector& outSVec,FVector& outEVec);
	bool SearchInteractingTarget(TScriptInterface<IEscapeRoomInteractable>& outActor);

	//UFUNCTION(BlueprintImplementableEvent)
	//void CreateInspectWidget(const TScriptInterface<IEscapeRoomInteractable>& inInspectingObject);

public:	
	UFUNCTION(BlueprintCallable)
	void SetInteractionLock(bool bLock);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(const FInputActionValue& Value);
	virtual void Interact_Implementation(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractEnd(const FInputActionValue& Value);
	virtual void InteractEnd_Implementation(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Inspect();
	virtual void Inspect_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InspectEnd();
	virtual void InspectEnd_Implementation();
};

