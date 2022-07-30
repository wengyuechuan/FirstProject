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
	TSubclassOf<class AItemStorage> WeaponStorage; //�������ǿ���ͨ������ȥ����һ�������������ͽ��������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	FVector CombatTargetLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UParticleSystem* HitParticles;//�˱������������������Ч

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }
	/** Set movement status and running speed */
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Enums")
	float StaminaDrainRate; //��������������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enums")
	float MinSprintStamina;  //��С����
	
	float InterpSpeed;

	bool bInterpToEnemy;

	void SetInterpToEnemy(bool Interp);//�����ת��
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
	//�������������⣬ÿ�ζ��ǣ�Defaults
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Play Stats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Play Stats")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Play Stats")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Play Stats")
	float Stamina;

	//��ƽ̨��ǿ���32λ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Play Stats")
	int32 Coins;



	TArray<FVector> PickupLocations;
	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	void DecrementHealth(float Amount);

	//UE4�е����ú��������ڲο�
	//���ص����˺�����
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

	//����һ���� Defaults
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AWeapon* EquippedWeapon;

	/**��ǰ�غϵ����壬��Ҫ�ж��Ƿ�װ��*/
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
	//ʹ�������������Լ��ٷ���
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

	void SwitchLevel(FName LevelName); //�ؿ�ת������

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="GameState")
	AGameStateBase* GameStateBase; //�����ҿ�������ͼ�����������

	FTimerHandle LevelTransTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer") //�ؿ��л���ʱ���ӳ�
	float TransDelay;

	UFUNCTION(BlueprintCallable)
	void LoadGameNoSwitch();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Loading")
	bool bLoadGamePush;

	void Empty();//һ���յĺ�������������ʱ��

};

/*
* ���һ��������Character ���Ƕ��ڹ���������Pawn
* ������ҿɲ��ݽ�ɫ��ʹ����Character ����Ҳ��������ĵĵط���
* CharacterĬ����Mesh ��һ��������ģ�� ͬʱҲ������һЩ���еĶ���
*/