#include "Interactable/EscapeRoomGrabComponent.h"

UEscapeRoomGrabComponent::UEscapeRoomGrabComponent() : TargetDist(300.0f)
{
}

void UEscapeRoomGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);

	if (!bHasToUpdateTarget) return;

	FVector SVec,EVec;
	FRotator SRot;
	GetOwner()->GetActorEyesViewPoint(SVec,SRot);
	EVec = SVec + SRot.Vector()*TargetDist;

	SetTargetLocation(EVec);
	//SetTargetRotation(SRot);
}


void UEscapeRoomGrabComponent::GrabActor(AActor* inActor)
{
	UPrimitiveComponent* PrimComp;

	if(!inActor || !(PrimComp = inActor->GetComponentByClass<UPrimitiveComponent>())) return;

	GrabComponentAtLocationWithRotation(PrimComp,NAME_None,inActor->GetActorLocation(),inActor->GetActorRotation());

	bHasToUpdateTarget = true;
}

void UEscapeRoomGrabComponent::ReleaseActor()
{	
	ReleaseComponent();

	bHasToUpdateTarget = false;
}
