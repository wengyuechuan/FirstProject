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

	//���ﴴ����һ���µĽ��棬�������ɿ�ʼ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/** Variable to hold the widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;
protected:
	virtual void BeginPlay() override;//��дBeginPlay����
};
