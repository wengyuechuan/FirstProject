// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//HUD���ʲ����� ����TSubclassOfҲ����˵��PlayerController�п��Ա�ѡ��
	//������Ҫ����ѧϰһ��
	//���Դ�TSubclassOf�������˵���ѡ��UClass
	/** Reference to the UMG asset in the editor*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/** Variable to hold the widget after creating it */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widgets")
	UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* EnemyHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WPauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* PauseMenu;

	bool bPauseMenuVisible;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category = "HUD")
	void DisplayPauseMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category = "HUD")
	void RemovePauseMenu();

	bool bEnemyHealthBarVisible;


	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	FVector EnemyLocation;
	void TogglePauseMenu(); //ȥ����Ƿ�˵��Ѿ�����

	void GamemodeOnly();
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
