// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FirstSaveGame.generated.h"
/*
* 用于用户的存档：
*	对于普通的数据，血量，体力都是可以直接保存的
* 而对于武器，是一个指针类型，无法通过简单的方式进行保存，那么下面将采取方式进行
* 我们在打开和关闭的过程中，我们发现不发生改变的就是我们存储的蓝图，这样我们可以创建一个蓝图专门来存储武器的信息
*/
USTRUCT(BlueprintType)
struct FCharacterStats { //在这里使用结构体，结构体的开头是用大写的F进行标志的
	GENERATED_BODY()
		//上面一句也是必须的
	UPROPERTY(VisibleAnywhere,Category="SaveGameData")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Stamina;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 Coins;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString WeaponName;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString LevelName;
};

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API UFirstSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UFirstSaveGame();

	UPROPERTY(VisibleAnywhere,Category=Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FCharacterStats CharacterStats; //保存一个结构体
};
