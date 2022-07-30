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
	//这里其创建的就是所谓的碰撞体，我们对于角色设计的碰撞体就是胶囊体的形状
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
	
	//在蓝图中可以调用
	//这里不需要在这里实现这个函数，我们在蓝图中实现，蓝图中红色的节点代表了事件
	//在C++中声明，而在蓝图中定义
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
* Timeline节点：给特定的变量创建平滑的过度
*/