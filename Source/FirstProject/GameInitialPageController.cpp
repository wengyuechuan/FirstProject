// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInitialPageController.h"
#include"Blueprint/UserWidget.h"
void AGameInitialPageController::BeginPlay() { //��дBeginPlay����
	Super::BeginPlay();//���ø����BeginPlay����

	if (HUDOverlayAsset) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	bShowMouseCursor = true;
}