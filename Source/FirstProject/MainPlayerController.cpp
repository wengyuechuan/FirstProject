// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include"Blueprint/UserWidget.h"
void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();
	
	if (HUDOverlayAsset) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar) {
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar) {
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Vec(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Vec);//水平对齐
	}
	if (WPauseMenu) {
		PauseMenu= CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu) {
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
void AMainPlayerController::DisplayEnemyHealthBar() {
	if (EnemyHealthBar) {
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}
void AMainPlayerController::RemoveEnemyHealthBar() {
	if (EnemyHealthBar) {
		bEnemyHealthBarVisible =false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
void AMainPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (EnemyHealthBar) {
		FVector2D PositionInViewport;
		//传递后会将我们传入的变量写入信息
		ProjectWorldLocationToScreen(EnemyLocation,PositionInViewport);//将三维的信息转换到屏幕上的二维
		PositionInViewport.Y -= 85.f;
		FVector2D SizeInViewport = FVector2D(300.f, 25.f);
		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);

	}
}
void AMainPlayerController::DisplayPauseMenu_Implementation() {
	if (PauseMenu) {
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputGameAndUI;
		SetInputMode(InputGameAndUI);//设置输入模式
		bShowMouseCursor = true;
	}
}
void AMainPlayerController::RemovePauseMenu_Implementation() {
	if (PauseMenu) {
		//FInputModeGameOnly InputGameOnly;
		//SetInputMode(InputGameOnly);//设置输入模式
		GamemodeOnly();
		bShowMouseCursor = false;

		bPauseMenuVisible = false;
	}
}
void AMainPlayerController::TogglePauseMenu() {
	if (bPauseMenuVisible) {
		RemovePauseMenu();
	}
	else {
		DisplayPauseMenu();
	}
}

void AMainPlayerController::GamemodeOnly() {
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}