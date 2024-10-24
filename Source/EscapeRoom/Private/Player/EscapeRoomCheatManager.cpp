// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EscapeRoomCheatManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

void UEscapeRoomCheatManager::TGM()
{
	// MAKE THIS TO ATTACHING STATE OBJ TO CHAR
	auto Char = UGameplayStatics::GetPlayerCharacter(this,0);

	if(!Char) return;

	auto CMC = Char->GetComponentByClass<UCharacterMovementComponent>();
	UActorComponent* FMC = Char->GetComponentByClass<UFloatingPawnMovement>();
	if(!FMC) FMC = Char->AddComponentByClass(UFloatingPawnMovement::StaticClass(),false,FTransform(),false);

	if(!CMC) return;

	if(CMC->IsActive())
	{
		CMC->SetActive(false,true);
		FMC->SetActive(true,true);
	}
	else
	{
		FMC->SetActive(false,true);
		CMC->SetActive(true,true);
	}
}
