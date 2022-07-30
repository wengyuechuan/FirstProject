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

	//���Խ�����ͼ�ν����޸�
	//ƴд��tm�������˰��죬������ MakeEditWidget
	UPROPERTY(EditAnywhere,meta=(MakeEditWidget="true"))
	FVector EndPoint;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Platform")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool bInterping;

	FTimerHandle InterpTimer;

	float Distance; //���ڼ��������յ�ľ���
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterping();

	void SwapVector(FVector&VecOne, FVector&VecTwo);
};
