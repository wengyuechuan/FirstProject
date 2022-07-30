// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include"Components/SPhereComponent.h"
#include"Particles/ParticleSystemComponent.h"
#include"Kismet/GameplayStatics.h"
#include"Engine/World.h"
#include"Sound/SoundCue.h" //��Ҫ���������
// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	SetRootComponent(CollisionVolume);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	//��ʼ������ϵͳ
	IdleParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent"));
	IdleParticleComponent->SetupAttachment(GetRootComponent());
	//ͨ����ͼ�������Ƿ�������ת�����������ò�ͬ�Ķ�����в�ͬ�Ķ���
	bRotate = false;
	RotationRate = 1.0f;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	//��һ�����Խ�����д�ĺ���
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ÿһ֡��������ת
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
	//�ڴ�����ʱ�������Ч
	// ����ĺ�������UGameplayStatics
	//SpawnEmitterAtLocation���԰����Ӱ󶨵�������

}
void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex) {
}