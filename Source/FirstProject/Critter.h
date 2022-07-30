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
	//�û��������
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//��̬����
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComponent;

	//�����
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;//ǰ���ʶclass ���߱�������֮�����������ļ� 

private:

	void MoveForward(float Value);
	void MoveRight(float Value);

	//���ڱ�־���°���֮����ƶ�����
	FVector CurrentVelocity;
	float maxspeed;
};
