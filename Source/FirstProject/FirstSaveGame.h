// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FirstSaveGame.generated.h"
/*
* �����û��Ĵ浵��
*	������ͨ�����ݣ�Ѫ�����������ǿ���ֱ�ӱ����
* ��������������һ��ָ�����ͣ��޷�ͨ���򵥵ķ�ʽ���б��棬��ô���潫��ȡ��ʽ����
* �����ڴ򿪺͹رյĹ����У����Ƿ��ֲ������ı�ľ������Ǵ洢����ͼ���������ǿ��Դ���һ����ͼר�����洢��������Ϣ
*/
USTRUCT(BlueprintType)
struct FCharacterStats { //������ʹ�ýṹ�壬�ṹ��Ŀ�ͷ���ô�д��F���б�־��
	GENERATED_BODY()
		//����һ��Ҳ�Ǳ����
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
	FCharacterStats CharacterStats; //����һ���ṹ��
};
