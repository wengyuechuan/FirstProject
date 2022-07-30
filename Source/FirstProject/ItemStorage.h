// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStorage.generated.h"

UCLASS()
class FIRSTPROJECT_API AItemStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemStorage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//这里也是需要理解的一个结构，就是所谓的TSubclassOf这样的结构 这个是在第二遍过代码的时候需要注意的地方
	//为什么使用一个map，因为这样可以通过一个名称去访问一个武器的资源
	UPROPERTY(EditDefaultsOnly,Category="SaveData")
	TMap<FString, TSubclassOf<class AWeapon>>  WeaponMap;
};
