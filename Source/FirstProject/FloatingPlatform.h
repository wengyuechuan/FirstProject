// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	/** Mesh for the platform*/
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Platform")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	FVector StartPoint;

	//可以进行在图形界面修改
	//拼写又tm错了整了半天，烦死了 MakeEditWidget
	UPROPERTY(EditAnywhere,meta=(MakeEditWidget="true"))
	FVector EndPoint;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Platform")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool bInterping;

	FTimerHandle InterpTimer;

	float Distance; //用于计算起点和终点的距离
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterping();

	void SwapVector(FVector&VecOne, FVector&VecTwo);
};
