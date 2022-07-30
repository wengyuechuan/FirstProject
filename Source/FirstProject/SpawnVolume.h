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

	//TSubclassOfҲ��Ҫ�Լ�ȥ���䣬��������˵�һ���ֵ�ѧϰ֮��ҪѸ�ٽ���ڶ����֣������������Ŀ��û��Ū���׵�֪ʶ�㣬����֮��Ŀ���
	//���ղ��������������Ļ��������������������˵��г��֣�Ҳ�������ǵı������Զ�����������
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

	//��ͼ�����¼�������ͼ��C++���������һ��������ͼ��һ������C++
	//��ʵ�൱�ڿ�������ͼ�н�����д������ͼ�ν��棬������ΪBlueprintNativeEvent��ʵ���ø�virtual���
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Spawning")
	void SpawnOurActor(UClass* ToSpawn,const FVector& Location);
};
/**
* ���NPC�����ǹ�����Խ��й��������ǶԻ���
* ������ʵ�����������ӣ�Ȼ���Һ�����ĳһ���㣬�������һ������
* ������ʵ�������������������Ĳ���
*/