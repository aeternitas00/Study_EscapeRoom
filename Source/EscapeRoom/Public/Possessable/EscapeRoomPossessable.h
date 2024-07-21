// Fill out your copyright notice in the Description page of Project Settings.

// ========================
//		DEPRECATED
// ========================

#pragma once

#include "EscapeRoom.h"
#include "UObject/Interface.h"
#include "EscapeRoomPossessable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEscapeRoomPossessable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IEscapeRoomPossessable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintImplementableEvent)
	USceneComponent* GetPreservedCameraSlot();
};
