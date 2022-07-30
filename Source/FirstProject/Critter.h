// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Critter.generated.h"

UCLASS()
class FIRSTPROJECT_API ACritter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACritter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//用户输入组件
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//静态网格
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComponent;

	//摄像机
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;//前面标识class 告诉编译器，之后会引入这个文件 

private:

	void MoveForward(float Value);
	void MoveRight(float Value);

	//用于标志按下按键之后的移动方向
	FVector CurrentVelocity;
	float maxspeed;
};
