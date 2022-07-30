// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class FIRSTPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Spawning")
	class UBoxComponent* SpawningBox;

	//TSubclassOf也需要自己去补充，这里完成了第一部分的学习之后要迅速进入第二部分，补充在这个项目中没有弄明白的知识点，辅助之后的开发
	//按照操作来看，这样的话，这个对象可以在下拉菜单中出现，也就是我们的变量可以定义成这个类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	TSubclassOf<class AActor> Actor_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<class AActor> Actor_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<class AActor> Actor_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<class AActor> Actor_4;

	TArray<TSubclassOf<AActor>>SpawnArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure,Category="Spawning")
	FVector GetSpawnPoint();

	UFUNCTION(BlueprintPure, Category = "Spawning")
	TSubclassOf<AActor> GetSpawnActor();

	//蓝图本地事件，将蓝图和C++结合起来，一部分用蓝图，一部分用C++
	//其实相当于可以在蓝图中进行重写，利用图形界面，声明成为BlueprintNativeEvent其实作用跟virtual差不多
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Spawning")
	void SpawnOurActor(UClass* ToSpawn,const FVector& Location);
};
/**
* 设计NPC或者是怪物可以进行攻击或者是对话等
* 这里其实是设计这个盒子，然后找盒子中某一个点，随机生成一个物体
* 这样就实现了我们随机生成物体的操作
*/