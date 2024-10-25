// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EscapeRoom.h"
#include "Possessable/EscapeRoomCharacterBase.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Ability/EscapeRoomAbilityComponent.h"
#include "Lars.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UEscapeRoomAbilityComponent;

UCLASS(Blueprintable, ClassGroup = (Ability), meta = (BlueprintSpawnableComponent))
class ESCAPEROOM_API UEscapeRoomLarsAbility : public UEscapeRoomAbilityComponent
{
	GENERATED_BODY()

public:
	TObjectPtr<ALars> OwnerLars;

public:
	// Sets default values for this component's properties
	UEscapeRoomLarsAbility();

	virtual void MainAbility_Implementation(const FInputActionInstance& Instance);
	virtual void SubAbility1_Implementation(const FInputActionInstance& Instance);
	virtual void SubAbility2_Implementation(const FInputActionInstance& Instance);
};

UCLASS()
class ESCAPEROOM_API ALars : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALars();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ToggleMagnetism();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ToggleChargeState();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ApplyForceBasedOnCharge(AActor* Actor, FVector ImpactPoint);
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<AActor> TargetedObject;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEscapeRoomLarsAbility> LarsAbilityComponent;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	bool bIsMagnetActive;
	enum class EChargeState { Positive, Negative } CurrentChargeState;
};
