// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInitialPageController.h"
#include"Blueprint/UserWidget.h"
void AGameInitialPageController::BeginPlay() { //重写BeginPlay函数
	Super::BeginPlay();//调用父类的BeginPlay函数

	if (HUDOverlayAsset) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	bShowMouseCursor = true;
}