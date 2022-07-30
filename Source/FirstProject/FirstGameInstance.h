// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FirstGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API UFirstGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UFirstGameInstance();

	bool bSwitchLevel;//�Ƿ�����˹ؿ��л�

	bool bNeedUpdateLocation;

	UPROPERTY(BlueprintReadWrite)
	bool bWin;//�Ƿ���ʤ��

	UPROPERTY(BlueprintReadOnly)
	FString WinText;
	UPROPERTY(BlueprintReadOnly)
	FString NotWinText;

	UPROPERTY(BlueprintReadWrite)
	bool bClickGameStart;//�������Ϸ��ʼ��ť�����ʱ��ͻ����±���Ȼ�󸲸�ԭ���Ĵ浵
};
