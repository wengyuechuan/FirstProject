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
		EnemyHealthBar->SetAlignmentInViewport(Vec);//ˮƽ����
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
		//���ݺ�Ὣ���Ǵ���ı���д����Ϣ
		ProjectWorldLocationToScreen(EnemyLocation,PositionInViewport);//����ά����Ϣת������Ļ�ϵĶ�ά
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
		SetInputMode(InputGameAndUI);//��������ģʽ
		bShowMouseCursor = true;
	}
}
void AMainPlayerController::RemovePauseMenu_Implementation() {
	if (PauseMenu) {
		//FInputModeGameOnly InputGameOnly;
		//SetInputMode(InputGameOnly);//��������ģʽ
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