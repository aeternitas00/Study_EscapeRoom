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
		//ƽ�� ��ġ�� �ް�
		FVector Location = GetActorLocation();
		float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float JourneyTravelled = (Location - GlobalStartLocation).Size();

		if (JourneyTravelled > JourneyLength)
		{
			FVector Swap = GlobalStartLocation;
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = Swap;
		}
		//���ڰ� ������ ��ȣ���ִ� SafeNormal
		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		// �ӵ� * �ʴ�ð� * ����
		Location += Speed * DeltaTime * Direction;
		//�ش� ��ġ�� �缳��
		SetActorLocation(Location);
	}
	
}
