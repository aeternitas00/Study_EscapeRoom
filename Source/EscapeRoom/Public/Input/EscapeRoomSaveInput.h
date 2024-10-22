// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EscapeRoomSaveInput.generated.h"
 

 // Like UPlayerMappableKeySettings ?
 USTRUCT(BlueprintType)
 struct ESCAPEROOM_API FEscapeRoomKeySettings
 {
	 GENERATED_BODY()



 };
/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UEscapeRoomSaveInput : public USaveGame
{
	GENERATED_BODY()
	
private:
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	

};
