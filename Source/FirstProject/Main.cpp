// Fill out your copyright notice in the Description page of Project Settings.
/*
* ���Ļ������ƣ�
* 1.��ʼ���������
* 2.������Ƶ����ƣ�������֮�������ȷ�ص��ط�
* 3.�ڶ��غ͵����صĵ�ͼ�еĲ���
* 4.���عؿ������
* 5.ʤ��֮��Ľ���
* 6.�浵���Ƶ��޸�������GameInstance�����޸����ҵ��뷨������GameInstance�洢��ǰ������Ƿ����˼��صİ�ť���������ԺܺõĽ������
*/

#include "Main.h"
#include"Components/CapsuleComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"Kismet/KismetSystemLibrary.h"
#include"Kismet/KismetMathLibrary.h"
#include"Weapon.h"
#include"Animation/AnimInstance.h"
#include"Sound/SoundCue.h"
#include"Kismet/GameplayStatics.h"
#include"Enemy.h"
#include"MainPlayerController.h"
#include"FirstSaveGame.h"
#include"CoinState.h"
#include"TimerManager.h"
#include"ItemStorage.h"
#include"FirstGameInstance.h"
// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Camera Boom(pulls towards the player if there's a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; //Camera follows at this distance
	CameraBoom->bUsePawnControlRotation = true; //Rotate arm based on controller ��������һ������Pawn������һ��������������Pawn���˶������д���һЩ��Ϣ �õ��ɱۿ������ſ�����ת��

	//Set
	GetCapsuleComponent()->SetCapsuleSize(45.f,72.f);
	//Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//�ڶ��������ǲ�۵�����
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);   //���������˲�۵ĸ��� ��ۿ��Էŵ��ʲ��ڣ�����һ���Ǽ� ������������и��ӣ���������Ҫ���������һ������һ��Ķ��������Ǿ���Ҫһ����ۣ����������뵽����
	//Attach the camera to the end of the boom and let the boom adjust the match
	//the controller orientation
	FollowCamera->bUsePawnControlRotation = false; //��ϣ��������ſ������ı仯���仯��Ҳ�������䲻�����ڿ�����
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;
	//��ɫ���������������ת����ת
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	bLMBDown = false;

	bESCDown = false;
	//Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; //Character moves in the direction of input..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);// ... at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 450.f;//The height of jumping
	GetCharacterMovement()->AirControl = 0.2f;// when character is in air, how much we can control it

	MaxHealth = 100.f;
	Health = 65.f;
	MaxStamina = 150.f;
	Stamina = 150.f;
	Coins = 0;

	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;
	bShiftKeyDown = false;
	
	//Inittialize Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus= EStaminaStatus::ESS_Normal;

	//ÿ������25������
	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	bAttacking = false;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;
	bHasCombatTarget = false;

	bMovingForward = false;
	bMovingRight = false;
	TransDelay = 2.f;//2s��Ĭ���л��ؿ�ʱ��

	bESCDown = false;

	bLoadGamePush = false;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	UFirstGameInstance* FirstGameInstance = Cast<UFirstGameInstance >(GetGameInstance());
	MainPlayerController = Cast<AMainPlayerController>(GetController());//ǿ��ת��
	GameStateBase = GetWorld()->GetGameState();//�õ���ǰ����Ϸ״̬
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));//�Ѵ浵ʵ�����ؽ���

	 if (Map != "SunTemple" && Map!="StartPage") {
		LoadGameNoSwitch(); //�������ɵ�����һ���ؿ�ʱ�������µ��ô˺�������ʱ�����һ��������Ǵ�ʱ��ת��������
	}
	 if (bLoadGamePush) {
		 UE_LOG(LogTemp, Warning, TEXT("hello"));
		 LoadGame(true);
		 bLoadGamePush = false;
	 }
	if (MainPlayerController && Map!="StartPage") {
		MainPlayerController->GamemodeOnly();
	}
	if (FirstGameInstance->bSwitchLevel) {
		UE_LOG(LogTemp, Warning, TEXT("%d"),FirstGameInstance->bSwitchLevel);
		SaveGame();//ÿ���л��ؿ���ʱ�򶼻ᱣ�浱ǰ�Ĵ浵
		FirstGameInstance->bSwitchLevel = false;
	}
	if (FirstGameInstance->bNeedUpdateLocation) { // ʵ�ֿ�ؿ�����λ��
		SetActorLocation(LoadGameInstance->CharacterStats.Location);
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
		FirstGameInstance->bNeedUpdateLocation = false;
	}
	if (FirstGameInstance->bClickGameStart) {
		SaveGame();
		FirstGameInstance->bClickGameStart = false;
	}
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	//�����ض�֡�ж�������
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus) {
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown) {
			if (Stamina - DeltaStamina <= MinSprintStamina) {
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}
			Stamina -= DeltaStamina;
			//�����������������ǣ�����������������С����Сֵʱ������һ������Ҳ���ǿ����ܣ����ǲ��ܣ�����Ҳ����Ϊ��ͳһ�������õ�
			if (bMovingForward || bMovingRight) {
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
			else {
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
		}
		else { //ShiftKeyUp ���״̬����Ҫ��ʼ�ظ���
			if (Stamina + DeltaStamina >= MaxStamina) {
				Stamina = MaxStamina;
			}
			else {
				Stamina += DeltaStamina; //��ʱ�Ϳ��Իظ���
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown) {
			if (Stamina - DeltaStamina <= 0.f) {
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted); //�����ľ�
				Stamina = 0;//������Ϊ0
				SetMovementStatus(EMovementStatus::EMS_Normal);//�ܲ�����
			}
			else {
				Stamina -= DeltaStamina;
				if (bMovingForward || bMovingRight) {
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else {
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
			}
		}
		else { //shift key up
			if (Stamina + DeltaStamina >= MinSprintStamina) {
				SetStaminaStatus(EStaminaStatus::ESS_Normal); //��ظ��ˣ����ͣ�
			}
			Stamina += DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Normal);//ȷ��û�����ܲ�
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown) {
			Stamina = 0.f;
		}
		else {
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);//���ڻظ�
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal); //ȷ���㲻���ܲ�
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina) {
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina+= DeltaStamina; //�����ڻظ�
		}
		else {
			Stamina += DeltaStamina;//���û�е��Ļ���Ҳ�ظ�
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);//��֤��û������
		break;
	default:;
	}
	if (bInterpToEnemy && CombatTarget) {
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);//��ֵ

		SetActorRotation(InterpRotation);
	}
	if (CombatTarget) {
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController) {
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}
FRotator AMain::GetLookAtRotationYaw(FVector Target) {
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//����Ƿ���ȷ������������쳣�����
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AMain::ShiftKeyDown);
	//����һ��Ҫ����ȷ����ҪŪ����
	PlayerInputComponent->BindAction("Sprint",IE_Released, this, &AMain::ShiftKeyUp);
	/*
	* �󶨵ĺ������Ǽ̳й�����
	*/
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	/*
	* ���ڼ��ESC����
	*/
	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMain::ESCDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMain::ESCUp);
	/*
	* ������¼�
	*/
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	/*
	* ���������������Ǽ̳й�����
	*/
	PlayerInputComponent->BindAxis("Turn", this, &AMain::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMain::LookUp);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpRate);

}

bool AMain::CanMove(float Value) {
	if (MainPlayerController) {
		FString Map = GetWorld()->GetMapName();
		Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		return Controller != nullptr &&
			Value != 0.0f &&
			(!bAttacking) &&
			(MovementStatus != EMovementStatus::EMS_Dead)&&
			(!MainPlayerController->bPauseMenuVisible) && (Map!="StartPage");
	}
	return false;
}

//�������Ա�֤�����ڽ���˵�״̬ʱ�����������
void AMain::Turn(float Value) {
	if (CanMove(Value)) {
		AddControllerYawInput(Value);
	}
}

void AMain::LookUp(float Value) {
	if (CanMove(Value)) {
		AddControllerPitchInput(Value);
	}
}

/** Called for forwards/backwards input*/
void AMain::MoveForward(float Value) {
	bMovingForward = false;
	if (CanMove(Value)) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();//�õ���ǰ����������ת�Ƕ�
		//YawΪˮƽ�������ת
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//���������YawRotation��ȷ��ǰ���ķ����ý�ɫ�����˶�
		//����֪ʶ ��ת���� ---���Լ�ȥ����
		//��ȡX������˶�
		//ͨ�����󹹽�Ȼ���ȡ�ض�������ƶ� ��ȡ�ض������ʸ��
		//�����µ�ʸ��Ȼ�󹹽�����Ȼ���õ���ǰ�����ʸ������λʸ����
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);

		bMovingForward = true;
	}
}

/** Called for side to side input*/
void AMain::MoveRight(float Value) {
	bMovingRight = false;
	if (CanMove(Value)) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();//�õ���ǰ����������ת�Ƕ�
		//YawΪˮƽ�������ת
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//���������YawRotation��ȷ��ǰ���ķ����ý�ɫ�����˶�
		//����֪ʶ ��ת���� ---���Լ�ȥ����
		//��ȡX������˶�
		//ͨ�����󹹽�Ȼ���ȡ�ض�������ƶ� ��ȡ�ض������ʸ��
		//�����µ�ʸ��Ȼ�󹹽�����Ȼ���õ���ǰ�����ʸ������λʸ����
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //������ҵ�ʸ��

		AddMovementInput(Direction, Value);
		bMovingRight = true;
	}

}

void AMain::TurnAtRate(float Rate) {
	//Ĭ����ÿ��65�㣬Rate�Ǳ��ʵ���ֵ��Ȼ������֡��ʱ��Ϳ��Եõ���ǰ֡��Ҫ���ٶ���
	//�ÿ�������ˮƽ����������ת
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void AMain::LookUpRate(float Rate) {
	//��ת�Ƕ�
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds()); 
}
void AMain::DecrementHealth(float Amount) {
	if (MovementStatus == EMovementStatus::EMS_Dead) return;//���ȥ���˾������ȥ������Ҳ����ȥ����
	if (Health - Amount <= 0.f) {
		Health = 0;//������Health-=Amount
		Die();
	}
	else {
		Health -= Amount;
	}
}
void AMain::Die() {
	if (MovementStatus == EMovementStatus::EMS_Dead) {
		return;
	}
	UAnimInstance *AnimaInstance = GetMesh()->GetAnimInstance();
	if (AnimaInstance && CombatMontage) {
		AnimaInstance->Montage_Play(CombatMontage, 2.2f);//�������������ͼ�н������õ�
		//AnimaInstance->Montage_JumpToSection(FName("Death"), CombatMontage);//���ò��ŵ�����
		AnimaInstance->Montage_JumpToSection(FName("Death"));
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AMain::Empty, 2.f);
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);//��ɫ����
	FName initName = "SunTemple";
	UGameplayStatics::OpenLevel(GetWorld(), initName);//ת�����µĹؿ�
}
void AMain::Jump() {
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (Map == "StartPage") return;
	if (MainPlayerController) {
		if (MainPlayerController->bPauseMenuVisible) return;
	}
	if (MovementStatus != EMovementStatus::EMS_Dead) {
		Super::Jump();
	}
}
void AMain::IncrementCoins(int32 Amount) {
	Coins += Amount;
	ACoinState *CoinState = Cast<ACoinState>(GameStateBase);
	if (CoinState) {
		CoinState->Coins += Amount;
		if (CoinState->Coins == CoinState->TargetCoins) {
			UFirstGameInstance *FirstGameInstance = Cast<UFirstGameInstance >(GetGameInstance());
			FirstGameInstance->bSwitchLevel = true;
			UWorld* World = GetWorld();
			FString Map = GetWorld()->GetMapName();
			Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
			if (Map == TEXT("FrozenCove")) {
				FirstGameInstance->bWin = true;//ʤ��
			}
			SwitchLevel(CoinState->NextLevelName);
		}
	}
}
void AMain:: IncrementHealth(float Amount) {
	if (Health + Amount >= MaxHealth) {
		Health = MaxHealth;
	}
	else {
		Health += Amount;
	}
}
void AMain::SetMovementStatus(EMovementStatus Status) {
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting) {
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}
void AMain::ShiftKeyDown() {
	bShiftKeyDown = true;
}
void AMain::ShiftKeyUp() {
	bShiftKeyDown = false;
}
void AMain::ShowPickupLocations() {
	//ʹ�õ��������з���
	for (FVector location : PickupLocations) {
		//��ӡ��ÿһ��ʰȡ���
		//�ڳ����л������壬12�Σ���ɫΪ��ɫ������ʱ��5�룬�ߵĴ�ϸΪ0.5�����ڵ���ʹ��
		UKismetSystemLibrary::DrawDebugSphere(this, location,25.f, 12, FLinearColor::Red, 10.f, .5f);
	}

}
void AMain::LMBDown() {
	bLMBDown = true;
	if (MovementStatus == EMovementStatus::EMS_Dead) {
		return;//����ֱ�ӽ���
	}
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (Map == "StartPage") return;

	if (MainPlayerController) {
		if (MainPlayerController->bPauseMenuVisible) return;
	}

	if (ActiveOverlappingItem) {
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon) {
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);//���ã���ֹ�뿪֮���з�Ӧ
		}
	}
	else if(EquippedWeapon) {
		Attack();
	}
}
void AMain::LMBUp() {
	bLMBDown = false;
}
void AMain::ESCDown() {
	bESCDown = true;

	if (MainPlayerController) {
		MainPlayerController->TogglePauseMenu();
	}
}
void AMain::ESCUp() {
	bESCDown = false;
}
void AMain::SetEquippedWeapon(AWeapon* WeaponSet) {
	if (EquippedWeapon) {
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponSet;
}
void AMain::Attack() {
	if (!bAttacking && (MovementStatus!=EMovementStatus::EMS_Dead)) {
		bAttacking = true;
		SetInterpToEnemy(true);

		UAnimInstance* AnimaInstance = GetMesh()->GetAnimInstance();
		if (AnimaInstance && CombatMontage) {
			int32 Selection = FMath::RandRange(0, 1);
			switch(Selection) {
				case 0:
					AnimaInstance->Montage_Play(CombatMontage, 2.2f);//�������������ͼ�н������õ�
					AnimaInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);//���ò��ŵ�����
					break;
				case 1:
					AnimaInstance->Montage_Play(CombatMontage, 1.8f);//�������������ͼ�н������õ�
					AnimaInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);//���ò��ŵ�����
					break;
				default:
					;
			}
		}
	}
}
void AMain::AttackEnd() {
	bAttacking = false;
	SetInterpToEnemy(false); //�������Զ�ת��
	if (bLMBDown) {
		Attack();
	}
}
void AMain::PlaySwingSound() {
	if (EquippedWeapon->SwingSound) {
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);//�Ӷ�������
	}
}
void AMain::SetInterpToEnemy(bool Interp) {
	bInterpToEnemy = Interp;
}
float AMain::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	if (Health - DamageAmount <= 0.f) {
		Health = 0;
		Die();
		if (DamageCauser) {
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			if (Enemy) {
				Enemy->bHasValidTarget = false;
			}
		}
	}
	else {
		Health -= DamageAmount;
	}
	return DamageAmount;
}
void AMain::DeathEnd() {
	GetMesh()->bPauseAnims = true;//������ͣ
	GetMesh()->bNoSkeletonUpdate = true;
}
void AMain::UpdateCombatTarget() {
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);
	
	if ((OverlappingActors.Num()) == 0) {
		if (MainPlayerController) {
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}
	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy) {
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();
		for (auto Actor : OverlappingActors) {
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy) {
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				if (DistanceToActor < MinDistance) {
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
		}
		if (MainPlayerController) {
			MainPlayerController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
}
void AMain::SwitchLevel(FName LevelName) {
	UWorld* World = GetWorld();
	FString Map = GetWorld()->GetMapName();
	if (Map != "StartPage") SaveGame(); //����֮ǰ���ȱ�����Ϸ
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (World) {
		FName CurrentLevelName(*Map);//�൱�ڽ����������� ��FStringת��ΪFName
		if (CurrentLevelName!= LevelName) {
			UGameplayStatics::OpenLevel(World, LevelName);//ת�����µĹؿ�
		}
	}

}
void AMain::SaveGame() {
	UFirstSaveGame* SaveGameInstance=Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	
	SaveGameInstance->CharacterStats.Health = Health;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Stamina= Stamina;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
	SaveGameInstance->CharacterStats.Coins= Coins;

	FString MapName=GetWorld()->GetMapName();
	//UE_LOG(LogTemp, Warning, TEXT("MapName:%s"),*MapName);
	//������Ҫȥ�����Ƶ�ǰ׺
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);//ֱ��ȥ��ǰ׺�����к������Ե���

	SaveGameInstance->CharacterStats.LevelName = MapName;
	if (EquippedWeapon) {
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name; //�����ƽ��и�ֵ
	}

	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);//д����

}

void AMain::LoadGame(bool SetLocation){
	UE_LOG(LogTemp, Warning, TEXT("Loading"));
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	
	UWorld* World = GetWorld();
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	LoadGameInstance=Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));//�Ѵ浵ʵ�����ؽ���
	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Stamina= LoadGameInstance->CharacterStats.Stamina;
	MaxStamina= LoadGameInstance->CharacterStats.MaxStamina;
	Coins= LoadGameInstance->CharacterStats.Coins;

	//װ������
	if (WeaponStorage) {
		AItemStorage * Weapons=GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if (Weapons) {
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
			if (Weapons->WeaponMap.Contains(WeaponName) && WeaponName!=TEXT("")) { //�ж��Ƿ��е�ǰ�ؼ��ֵ�ֵ
				AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
				WeaponToEquip->Equip(this);//�����װ������
			}
		}
	}
	if (World) {
		FName CurrentLevelName(*Map);//�൱�ڽ����������� ��FStringת��ΪFName
		if (*Map != LoadGameInstance->CharacterStats.LevelName) {
			UFirstGameInstance* FirstGameInstance = Cast<UFirstGameInstance >(GetGameInstance());
			FirstGameInstance->bNeedUpdateLocation = true;
		}
	}

	if (SetLocation) {
		SetActorLocation(LoadGameInstance->CharacterStats.Location);
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
	}
	if (LoadGameInstance->CharacterStats.LevelName != TEXT("")) {
		FName LevelName(*LoadGameInstance->CharacterStats.LevelName);
		SwitchLevel(LevelName);
	}
	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
	bLoadGamePush = true;
}

void AMain::LoadGameNoSwitch() {
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));//�Ѵ浵ʵ�����ؽ���

	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Stamina = LoadGameInstance->CharacterStats.Stamina;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
	Coins = LoadGameInstance->CharacterStats.Coins;

	//װ������
	if (WeaponStorage) {
		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if (Weapons) {
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
			if (Weapons->WeaponMap.Contains(WeaponName) && WeaponName != TEXT("")) { //�ж��Ƿ��е�ǰ�ؼ��ֵ�ֵ
				AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
				WeaponToEquip->Equip(this);//�����װ������
			}
		}
	}



	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
}
void AMain::Empty() {
	
}