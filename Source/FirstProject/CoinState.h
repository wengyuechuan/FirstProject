// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CoinState.generated.h"

/**
 * �Լ��Ķ������Ϸ״̬���������������������Ϸ������Ȼ���������
 */
UCLASS()
class FIRSTPROJECT_API ACoinState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ACoinState();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Coins")
	int Coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coins")
	int TargetCoins;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LevelName")
	FName NextLevelName;
};
