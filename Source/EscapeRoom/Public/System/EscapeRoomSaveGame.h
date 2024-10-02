// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EscapeRoomSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UEscapeRoomSaveGame : public USaveGame
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentStageIdx;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentChapterIdx;
};
