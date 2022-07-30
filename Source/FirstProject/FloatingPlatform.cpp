// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include"Components/StaticMeshComponent.h"
#include"TimerManager.h"
/*
* 需要使用插值去完成我们的操作
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
	StartPoint = GetActorLocation();//得到初始的位置
	//在图形化界面中显示的是终点的相对位置，我们需要加上初始点才可以得到
	EndPoint += StartPoint;
	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
	//底层重写了减法，让其相减之后得到的仍然是矢量，所以我们通过.Size()进行调用，可以得到距离
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping) {
		FVector CurrentLocation = GetActorLocation();

		//这里根据时间得到了起点和终点附近的各个点的坐标
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