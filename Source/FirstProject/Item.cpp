// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include"Components/SPhereComponent.h"
#include"Particles/ParticleSystemComponent.h"
#include"Kismet/GameplayStatics.h"
#include"Engine/World.h"
#include"Sound/SoundCue.h" //需要导入这个库
// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	SetRootComponent(CollisionVolume);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	//初始化粒子系统
	IdleParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent"));
	IdleParticleComponent->SetupAttachment(GetRootComponent());
	//通过蓝图来控制是否会进行旋转，这样可以让不同的对象具有不同的动作
	bRotate = false;
	RotationRate = 1.0f;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	//绑定一个可以进行重写的函数
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//每一帧都进行旋转
	if (bRotate) {
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex,
	bool bFromSweep, const 
	FHitResult& SweepResult) {
	//在触碰的时候出现特效
	// 下面的函数属于UGameplayStatics
	//SpawnEmitterAtLocation可以把粒子绑定到物体上

}
void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex) {
}