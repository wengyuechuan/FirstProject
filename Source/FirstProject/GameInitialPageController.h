// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameInitialPageController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AGameInitialPageController : public APlayerController
{
	GENERATED_BODY()
public:

	//这里创建了一个新的界面，用于容纳开始界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/** Variable to hold the widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;
protected:
	virtual void BeginPlay() override;//重写BeginPlay函数
};
