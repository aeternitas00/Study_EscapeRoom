// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMove) 
	{
		//틱당 위치를 받고
		FVector Location = GetActorLocation();
		float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float JourneyTravelled = (Location - GlobalStartLocation).Size();

		if (JourneyTravelled > JourneyLength)
		{
			FVector Swap = GlobalStartLocation;
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = Swap;
		}
		//숫자가 작을때 보호해주는 SafeNormal
		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		// 속도 * 초당시간 * 방향
		Location += Speed * DeltaTime * Direction;
		//해당 위치를 재설정
		SetActorLocation(Location);
	}
	
}
