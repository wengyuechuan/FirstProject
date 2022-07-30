// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();
	/** Overlap volume for functionality to be triggered */
	//�����䴴���ľ�����ν����ײ�壬���Ƕ��ڽ�ɫ��Ƶ���ײ����ǽ��������״
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Floor Switch")
	class UBoxComponent* TriggerBox;

	/** Switch for the character to step on*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	class UStaticMeshComponent* FloorSwitch;

	/**Door to move when the floor switch is stepped on*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	UStaticMeshComponent* Door;

	/** Initial location for the door*/
	UPROPERTY(BlueprintReadWrite,Category="Floor Switch")
	FVector InitialDoorLocation;

	/** Initial location for the door */
	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
	FVector InitialSwitchLocation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Floor Switch")
	float SwitchTime;

	void CloseDoor();
	FTimerHandle SwitchHandle;
	bool bCharacterOnSwitch;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//����ͼ�п��Ե���
	//���ﲻ��Ҫ������ʵ�������������������ͼ��ʵ�֣���ͼ�к�ɫ�Ľڵ�������¼�
	//��C++��������������ͼ�ж���
	UFUNCTION(BlueprintImplementableEvent,Category="Floor Switch")
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerFloorSwitch();

	UFUNCTION(BlueprintCallable,Category="Floor Switch")
	void UpdateDoorLocation(float z);

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdateFloorSwitchLocation(float z);

};

/*
* Timeline�ڵ㣺���ض��ı�������ƽ���Ĺ���
*/