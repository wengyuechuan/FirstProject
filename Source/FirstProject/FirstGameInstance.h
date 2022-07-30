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

	bool bSwitchLevel;//是否进行了关卡切换

	bool bNeedUpdateLocation;

	UPROPERTY(BlueprintReadWrite)
	bool bWin;//是否获得胜利

	UPROPERTY(BlueprintReadOnly)
	FString WinText;
	UPROPERTY(BlueprintReadOnly)
	FString NotWinText;

	UPROPERTY(BlueprintReadWrite)
	bool bClickGameStart;//点击了游戏开始按钮，这个时候就会重新保存然后覆盖原本的存档
};
