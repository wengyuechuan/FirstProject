// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstSaveGame.h"

UFirstSaveGame::UFirstSaveGame() {
	PlayerName=TEXT("Default");
	UserIndex=0; //玩家的序号

	CharacterStats.WeaponName = TEXT("");//默认是空串，如果是空串的代表没有装备武器
	CharacterStats.LevelName = TEXT("");
}