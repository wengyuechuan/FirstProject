// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include"Components/StaticMeshComponent.h"
#include"TimerManager.h"
/*
* ��Ҫʹ�ò�ֵȥ������ǵĲ���
*/
// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);
	InterpSpeed = 4.0f;
	bInterping = false;
	InterpTime = 1.f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();//�õ���ʼ��λ��
	//��ͼ�λ���������ʾ�����յ�����λ�ã�������Ҫ���ϳ�ʼ��ſ��Եõ�
	EndPoint += StartPoint;
	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
	//�ײ���д�˼������������֮��õ�����Ȼ��ʸ������������ͨ��.Size()���е��ã����Եõ�����
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping) {
		FVector CurrentLocation = GetActorLocation();

		//�������ʱ��õ��������յ㸽���ĸ����������
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);

		SetActorLocation(Interp);

		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTraveled <= 1.f) {
			ToggleInterping();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			SwapVector(StartPoint, EndPoint);
		}
	}

}

void AFloatingPlatform::ToggleInterping() {
	bInterping = !bInterping;
}
void AFloatingPlatform::SwapVector(FVector& VecOne, FVector& VecTwo) {
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}