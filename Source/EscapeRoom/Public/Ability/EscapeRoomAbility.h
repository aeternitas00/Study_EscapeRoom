// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EscapeRoomAbility.generated.h"

/**
 * Example code
 * 
 * ========= DEPRECATED =========
 */
UCLASS(Blueprintable,Abstract)
class ESCAPEROOM_API UEscapeRoomAbility : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	bool ActivateAbility(/*~~*/);	
	virtual bool ActivateAbility_Implementation(/*~~*/){return false;};

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void EndAbility(/*~~*/);	
	virtual void EndAbility_Implementation(/*~~*/){};

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float SomeAbilityDatas;
};


//UCLASS(Blueprintable,Abstract)
//class ESCAPEROOM_API UEscapeRoomAbility_Death : public UEscapeRoomAbility
//{
//	GENERATED_BODY()
//
//};
