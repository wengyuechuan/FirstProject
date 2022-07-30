// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ColliderMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API UColliderMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	/*
	* ��һ���ǴӼ̳й�ϵ��Ѱ���ģ����Ա���д
	*/
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
