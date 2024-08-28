// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EscapeRoom.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "EscapeRoomGrabComponent.generated.h"

UCLASS( ClassGroup=(Interact), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UEscapeRoomGrabComponent : public UPhysicsHandleComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	float TargetDist;

	bool bHasToUpdateTarget;
public:
	UEscapeRoomGrabComponent();
protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; 
public:	

	UFUNCTION(BlueprintCallable)
	void GrabActor(AActor* inActor);

	UFUNCTION(BlueprintCallable)
	void ReleaseActor();
};
