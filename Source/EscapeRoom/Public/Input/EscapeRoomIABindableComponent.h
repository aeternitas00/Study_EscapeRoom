// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EscapeRoom.h"
#include "Components/ActorComponent.h"
#include "InputTriggers.h"
#include "EscapeRoomIABindableComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class UEnhancedInputComponent;
class UEnhancedInputLocalPlayerSubsystem;

struct FInputActionInstance;

USTRUCT(BlueprintType)
struct ESCAPEROOM_API FEscapeRoomInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<const UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TSet<ETriggerEvent> BindingEvents;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UEscapeRoomIABindableComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<const UInputMappingContext> ComponentMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	int32 MappingContextPriority = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TMap<FGameplayTag, TObjectPtr<const UInputAction>> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TMap<FGameplayTag, FEscapeRoomInputAction> DynamicInputActions;

protected:
	virtual void BindNativeActions(UEnhancedInputComponent* InputComponent);

	UFUNCTION(BlueprintNativeEvent)
	void SendTaggedInputToOwner(const FInputActionInstance& Instance, const FGameplayTag& InputTag);
	virtual void SendTaggedInputToOwner_Implementation(const FInputActionInstance& Instance, const FGameplayTag& InputTag);
public:
	void BindActions(UEnhancedInputComponent* InputComponent);
	void AddMappingContext(UEnhancedInputLocalPlayerSubsystem* Subsystem);
	void RemoveMappingContext(UEnhancedInputLocalPlayerSubsystem* Subsystem);
};
