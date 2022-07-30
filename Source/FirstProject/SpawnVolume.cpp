// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include"Components/BoxComponent.h"
#include"Engine/Classes/Kismet/KismetMathLibrary.h"
#include"Engine/World.h"
#include"GameFramework/Actor.h"
#include"Enemy.h"
#include "AIController.h"
// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	if (Actor_1 && Actor_2 && Actor_3 && Actor_4) {
		SpawnArray.Add(Actor_1);
		SpawnArray.Add(Actor_2);
		SpawnArray.Add(Actor_3);
		SpawnArray.Add(Actor_4);
	}
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
FVector ASpawnVolume::GetSpawnPoint() {
	//随机生成位置
	FVector Extent = SpawningBox->GetScaledBoxExtent(); //包含了盒子的大小信息
	FVector Origin = SpawningBox->GetComponentLocation();//盒子的原点

	//计算盒子中的随机点
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	return Point;
}

void ASpawnVolume::SpawnOurActor_Implementation(UClass* ToSpawn, const FVector& Location) {
	if (ToSpawn!=nullptr) {
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;

		if (World) {
			AActor * Actor=World->SpawnActor<AActor>(ToSpawn,Location,FRotator(0.f),SpawnParams);
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy) {
				Enemy->SpawnDefaultController();
				AAIController* AICont = Cast<AAIController>(Enemy->GetController());
				if (AICont) {
					Enemy->AIController = AICont;
				}
			}
		}
	}
}
TSubclassOf<AActor> ASpawnVolume::GetSpawnActor() {
	if (SpawnArray.Num() > 0) {
		int32 Selection = FMath::RandRange(0, SpawnArray.Num() - 1);
		
		return SpawnArray[Selection];
	}
	else {
		return nullptr;
	}
}