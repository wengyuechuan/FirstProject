// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstGameInstance.h"
UFirstGameInstance::UFirstGameInstance() {
	bSwitchLevel = false;
	bNeedUpdateLocation = false;
	bWin = false;
	WinText = TEXT("恭喜您获得了胜利！");
	NotWinText = TEXT("");//空串
	bClickGameStart = false;
}
