// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"Camera/CameraComponent.h"
#include"GameFramework/SpringArmComponent.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus :uint8 { 
	EMS_Normal UMETA(DisplayName="Normal"),
	EMS_Sprinting UMETA(DisplayName="Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),
	EMS_MAX UMETA(DisplayName="DefaultMAX")
};
UENUM(BlueprintType)
enum class EStaminaStatus :uint8{
	ESS_Normal UMETA(DisplayName="Normal"),
	ESS_BelowMinimum UMETA(DisplayName="BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName="Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName="ExhaustedRecovering"),
	ESS_MAX UMETA(DisplayName="DefaultMax")
};
UCLASS()
class FIRSTPROJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	UPROPERTY(EditDefaultsOnly,Category="SaveData")
	TSubclassOf<class AItemStorage> WeaponStorage; //这样我们可以通过名字去访问一个武器，这样就解决了问题

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	FVector CombatTargetLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UParticleSystem* HitParticles;//人被攻击的情况的粒子特效

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }
	/** Set movement status and running speed */
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Enums")
	float StaminaDrainRate; //体力条消耗速率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enums")
	float MinSprintStamina;  //最小下限
	
	float InterpSpeed;

	bool bInterpToEnemy;

	void SetInterpToEnemy(bool Interp);//向敌人转向
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Combat")
	class AEnemy* CombatTarget;

	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget=Target; }
	/** Camera boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CameraBoom;

	/** Follow Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/**Base turn rates to scale turning functions for the camera*/
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/**
	* Player Stats
	*/
	//又是这样的问题，每次都是，Defaults
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Play Stats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Play Stats")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Play Stats")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Play Stats")
	float Stamina;

	//跨平台的强大的32位整数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Play Stats")
	int32 Coins;



	TArray<FVector> PickupLocations;
	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	void DecrementHealth(float Amount);

	//UE4中的内置函数，用于参考
	//返回的是伤害数量
	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator,AActor* DamageCauser) override; 

	void Die();

	virtual void Jump() override;

	UFUNCTION(BlueprintCallable)
	void IncrementCoins(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void IncrementHealth(float Amount);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Running")
	float RunningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintingSpeed;

	bool bShiftKeyDown;

	void ShiftKeyDown();
	void ShiftKeyUp();

	//又来一次是 Defaults
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AWeapon* EquippedWeapon;

	/**当前重合的物体，需要判断是否装备*/
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Items")
	class AItem* ActiveOverlappingItem;

	void SetEquippedWeapon(AWeapon* WeaponSet);
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Combat")
	bool bHasCombatTarget;
	
	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }

	FORCEINLINE AWeapon* GetEquipperdWeapon() { return EquippedWeapon; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Called for forwards/backwards input*/
	void MoveForward(float Value);
	
	/** Called for side to side input*/
	void MoveRight(float Value);

	/** Called for Yaw rotation*/
	void Turn(float Value);

	/** Called for pitch rotation*/
	void LookUp(float Value);

	bool bMovingForward;
	bool bMovingRight;

	bool CanMove(float Value);
	/** Called via input to turn at a given rate
	* @param Rate This is a normalized rate,i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/// <summary>
	/// Called via input to look up/down at a given rate
	/// </summary>
	/// <param name="Rate">This is a normalized rate,i.e. 1.0 means 100% of desired look up and down rate</param>
	void LookUpRate(float Rate);
	FRotator GetLookAtRotationYaw(FVector Target);
	bool bLMBDown;
	void LMBDown();
	void LMBUp();

	bool bESCDown;
	void ESCDown();
	void ESCUp();
	//For getter and setter,we often use inline function
	//使用内联函数可以加速访问
	FORCEINLINE USpringArmComponent* GetCameraBoom()const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Anims")
	bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class USoundCue* HitSound;

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	void UpdateCombatTarget();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Combat")
	TSubclassOf<AEnemy>   EnemyFilter;

	void SwitchLevel(FName LevelName); //关卡转换函数

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="GameState")
	AGameStateBase* GameStateBase; //这样我可以在蓝图里面进行设置

	FTimerHandle LevelTransTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer") //关卡切换的时间延迟
	float TransDelay;

	UFUNCTION(BlueprintCallable)
	void LoadGameNoSwitch();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Loading")
	bool bLoadGamePush;

	void Empty();//一个空的函数，用于拖延时间

};

/*
* 最后一个部分是Character 我们对于怪物利用了Pawn
* 对于玩家可操纵角色，使用了Character 这里也是最激动人心的地方了
* Character默认有Mesh 是一个骨骼的模型 同时也创建了一些已有的东西
*/