// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/EscapeRoomInteractionComponent.h"
#include "Interactable/EscapeRoomInteractable.h"

#include "KismetTraceUtils.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EscapeRoomTags.h"


// Sets default values for this component's properties
UEscapeRoomInteractionComponent::UEscapeRoomInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEscapeRoomInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UEscapeRoomInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!InteractionLocked)
	{	
		TScriptInterface<IEscapeRoomInteractable> newInspectingTarget;

		SearchInteractingTarget(newInspectingTarget);

		if ( InspectingObject != newInspectingTarget ) 
		{
			InspectEnd();
			InspectingObject = newInspectingTarget;
			Inspect();
		}
	}
}

void UEscapeRoomInteractionComponent::BindActions(UEnhancedInputComponent* InputComponent)
{
	if(!NativeInputActions.Find(EscapeRoomTags::InputTag_Interact)) return;

	InputComponent->BindAction(NativeInputActions[EscapeRoomTags::InputTag_Interact], ETriggerEvent::Started, this, &ThisClass::Interact);
	InputComponent->BindAction(NativeInputActions[EscapeRoomTags::InputTag_Interact], ETriggerEvent::Completed, this, &ThisClass::InteractEnd);
}

void UEscapeRoomInteractionComponent::GetTracePoints(FVector& outSVec, FVector& outEVec)
{
	if(GetOwner()->IsA<AController>()){
		FRotator SRot;
		Cast<AController>(GetOwner())->GetPlayerViewPoint(outSVec,SRot);
		outEVec = outSVec + SRot.Vector()*800.0f;
	}

	//else if(GetOwner()->IsA<ACharacter>()){
		//FRotator SRot;
		//Cast<ACharacter>(GetOwner())->GetEyePoint(outSVec,SRot);
		//outEVec = SRot.Vector()*800.0f;
	//}
}

bool UEscapeRoomInteractionComponent::SearchInteractingTarget(TScriptInterface<IEscapeRoomInteractable>& outActor)
{
	// Search Interactable

	FVector SVec,EVec;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Cast<AActor>(InteractingObject.GetObject()));

	GetTracePoints(SVec,EVec);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, SVec, EVec, ECC_GameTraceChannel1, CollisionParams);

	DrawDebugLineTraceSingle(GetWorld(), SVec, EVec,EDrawDebugTrace::ForOneFrame,bHit,HitResult,FLinearColor::Red,FLinearColor::Green,5.0f);
	if(bHit && HitResult.GetActor()->Implements<UEscapeRoomInteractable>())
	{
		outActor = HitResult.GetActor();
		return true;
	}

	return false;
}

void UEscapeRoomInteractionComponent::SetInteractionLock(bool bLock)
{
	InteractionLocked = bLock;

	if (InteractionLocked && InspectingObject.GetObject()) IEscapeRoomInteractable::Execute_EndInspect(InspectingObject.GetObject(),GetOwner());
}

void UEscapeRoomInteractionComponent::Interact_Implementation(const FInputActionValue& Value)
{
	if(!InspectingObject.GetObject()) return;
	
	SetInteractionLock(true);

	InteractingObject = InspectingObject;

	IEscapeRoomInteractable::Execute_StartInteract(InteractingObject.GetObject(),GetOwner());

}

void UEscapeRoomInteractionComponent::InteractEnd_Implementation(const FInputActionValue& Value)
{
	SetInteractionLock(false);

	if(!InteractingObject.GetObject()) return;

	IEscapeRoomInteractable::Execute_EndInteract(InteractingObject.GetObject(),GetOwner());

	InteractingObject = nullptr;
}

void UEscapeRoomInteractionComponent::Inspect_Implementation()
{
	if(!InspectingObject.GetObject()) return;
	IEscapeRoomInteractable::Execute_StartInspect(InspectingObject.GetObject(),GetOwner());
}

void UEscapeRoomInteractionComponent::InspectEnd_Implementation()
{
	if(!InspectingObject.GetObject()) return;
	IEscapeRoomInteractable::Execute_EndInspect(InspectingObject.GetObject(),GetOwner());
}

