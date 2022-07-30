// Fill out your copyright notice in the Description page of Project Settings.
/*
* 最后的机制完善：
* 1.开始界面的完善
* 2.复活机制的完善，即死亡之后如何正确回到地方
* 3.第二关和第三关的地图中的布置
* 4.隐藏关卡的设计
* 5.胜利之后的界面
* 6.存档机制的修复，利用GameInstance进行修复，我的想法是利用GameInstance存储当前的玩家是否点击了加载的按钮，这样可以很好的解决问题
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
	CameraBoom->bUsePawnControlRotation = true; //Rotate arm based on controller 控制器是一个随着Pawn创建的一个东西，会随着Pawn的运动向其中传递一些信息 让弹簧臂可以随着控制器转动

	//Set
	GetCapsuleComponent()->SetCapsuleSize(45.f,72.f);
	//Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//第二个变量是插槽的名称
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);   //这里引入了插槽的概念 插槽可以放到资产内，比如一个骨架 允许在上面进行附加，若我们需要给对象加入一个武器一类的东西，我们就需要一个插槽，将武器插入到其中
	//Attach the camera to the end of the boom and let the boom adjust the match
	//the controller orientation
	FollowCamera->bUsePawnControlRotation = false; //不希望相机随着控制器的变化而变化，也就是让其不依赖于控制器
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;
	//角色不会随着摄像机旋转而旋转
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

	//每秒消耗25点体力
	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	bAttacking = false;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;
	bHasCombatTarget = false;

	bMovingForward = false;
	bMovingRight = false;
	TransDelay = 2.f;//2s的默认切换关卡时间

	bESCDown = false;

	bLoadGamePush = false;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	UFirstGameInstance* FirstGameInstance = Cast<UFirstGameInstance >(GetGameInstance());
	MainPlayerController = Cast<AMainPlayerController>(GetController());//强制转换
	GameStateBase = GetWorld()->GetGameState();//得到当前的游戏状态
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));//把存档实例加载进来

	 if (Map != "SunTemple" && Map!="StartPage") {
		LoadGameNoSwitch(); //这样过渡到另外一个关卡时，会重新调用此函数，此时会出现一个现象就是此时会转换到那里
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
		SaveGame();//每次切换关卡的时候都会保存当前的存档
		FirstGameInstance->bSwitchLevel = false;
	}
	if (FirstGameInstance->bNeedUpdateLocation) { // 实现跨关卡更新位置
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
	//计算特定帧有多少消耗
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus) {
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown) {
			if (Stamina - DeltaStamina <= MinSprintStamina) {
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}
			Stamina -= DeltaStamina;
			//这里进行重设的理由是，当出现体力不够（小于最小值时）会有一个现象，也就是可以跑，但是不跑，这里也就是为了统一这样设置的
			if (bMovingForward || bMovingRight) {
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
			else {
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
		}
		else { //ShiftKeyUp 这个状态体力要开始回复了
			if (Stamina + DeltaStamina >= MaxStamina) {
				Stamina = MaxStamina;
			}
			else {
				Stamina += DeltaStamina; //此时就可以回复了
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown) {
			if (Stamina - DeltaStamina <= 0.f) {
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted); //体力耗尽
				Stamina = 0;//体力变为0
				SetMovementStatus(EMovementStatus::EMS_Normal);//跑不动了
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
				SetStaminaStatus(EStaminaStatus::ESS_Normal); //你回复了，快送！
			}
			Stamina += DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Normal);//确保没有在跑步
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown) {
			Stamina = 0.f;
		}
		else {
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);//正在回复
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal); //确认你不在跑步
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina) {
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina+= DeltaStamina; //我正在回复
		}
		else {
			Stamina += DeltaStamina;//如果没有到的话我也回复
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);//保证你没有在跑
		break;
	default:;
	}
	if (bInterpToEnemy && CombatTarget) {
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);//插值

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
	//检测是否正确，如果出现了异常则结束
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AMain::ShiftKeyDown);
	//这里一定要绑定明确，不要弄错了
	PlayerInputComponent->BindAction("Sprint",IE_Released, this, &AMain::ShiftKeyUp);
	/*
	* 绑定的函数都是继承过来的
	*/
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	/*
	* 用于检测ESC按键
	*/
	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMain::ESCDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMain::ESCUp);
	/*
	* 绑定左键事件
	*/
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	/*
	* 下面两个函数都是继承过来的
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

//这样可以保证我们在进入菜单状态时不会出现问题
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
		const FRotator Rotation = Controller->GetControlRotation();//得到当前控制器的旋转角度
		//Yaw为水平方向的旋转
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//利用上面的YawRotation，确定前进的方向让角色进行运动
		//利用知识 旋转矩阵 ---》自己去补充
		//获取X方向的运动
		//通过矩阵构建然后获取特定方向的移动 获取特定方向的矢量
		//基于新的矢量然后构建矩阵，然后获得到当前方向的矢量（单位矢量）
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
		const FRotator Rotation = Controller->GetControlRotation();//得到当前控制器的旋转角度
		//Yaw为水平方向的旋转
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//利用上面的YawRotation，确定前进的方向让角色进行运动
		//利用知识 旋转矩阵 ---》自己去补充
		//获取X方向的运动
		//通过矩阵构建然后获取特定方向的移动 获取特定方向的矢量
		//基于新的矢量然后构建矩阵，然后获得到当前方向的矢量（单位矢量）
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //获得向右的矢量

		AddMovementInput(Direction, Value);
		bMovingRight = true;
	}

}

void AMain::TurnAtRate(float Rate) {
	//默认是每秒65°，Rate是倍率的数值，然后乘这个帧的时间就可以得到当前帧需要多少度了
	//让控制器在水平方向上面旋转
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void AMain::LookUpRate(float Rate) {
	//旋转角度
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds()); 
}
void AMain::DecrementHealth(float Amount) {
	if (MovementStatus == EMovementStatus::EMS_Dead) return;//如果去世了就是真的去世了再也不能去世了
	if (Health - Amount <= 0.f) {
		Health = 0;//或者是Health-=Amount
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
		AnimaInstance->Montage_Play(CombatMontage, 2.2f);//这个是我们在蓝图中进行设置的
		//AnimaInstance->Montage_JumpToSection(FName("Death"), CombatMontage);//设置播放的内容
		AnimaInstance->Montage_JumpToSection(FName("Death"));
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AMain::Empty, 2.f);
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);//角色死亡
	FName initName = "SunTemple";
	UGameplayStatics::OpenLevel(GetWorld(), initName);//转换到新的关卡
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
				FirstGameInstance->bWin = true;//胜利
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
	//使用迭代器进行访问
	for (FVector location : PickupLocations) {
		//打印出每一个拾取物的
		//在场景中绘制球体，12段，颜色为红色，持续时间5秒，线的粗细为0.5，用于调试使用
		UKismetSystemLibrary::DrawDebugSphere(this, location,25.f, 12, FLinearColor::Red, 10.f, .5f);
	}

}
void AMain::LMBDown() {
	bLMBDown = true;
	if (MovementStatus == EMovementStatus::EMS_Dead) {
		return;//这样直接结束
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
			SetActiveOverlappingItem(nullptr);//重置，防止离开之后还有反应
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
					AnimaInstance->Montage_Play(CombatMontage, 2.2f);//这个是我们在蓝图中进行设置的
					AnimaInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);//设置播放的内容
					break;
				case 1:
					AnimaInstance->Montage_Play(CombatMontage, 1.8f);//这个是我们在蓝图中进行设置的
					AnimaInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);//设置播放的内容
					break;
				default:
					;
			}
		}
	}
}
void AMain::AttackEnd() {
	bAttacking = false;
	SetInterpToEnemy(false); //不启动自动转向
	if (bLMBDown) {
		Attack();
	}
}
void AMain::PlaySwingSound() {
	if (EquippedWeapon->SwingSound) {
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);//挥动的声音
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
	GetMesh()->bPauseAnims = true;//画面暂停
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
	if (Map != "StartPage") SaveGame(); //进入之前会先保存游戏
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (World) {
		FName CurrentLevelName(*Map);//相当于解引用这样的 将FString转化为FName
		if (CurrentLevelName!= LevelName) {
			UGameplayStatics::OpenLevel(World, LevelName);//转换到新的关卡
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
	//我们需要去掉名称的前缀
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);//直接去掉前缀这里有函数可以调用

	SaveGameInstance->CharacterStats.LevelName = MapName;
	if (EquippedWeapon) {
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name; //将名称进行赋值
	}

	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);//写入插槽

}

void AMain::LoadGame(bool SetLocation){
	UE_LOG(LogTemp, Warning, TEXT("Loading"));
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	
	UWorld* World = GetWorld();
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	LoadGameInstance=Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));//把存档实例加载进来
	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Stamina= LoadGameInstance->CharacterStats.Stamina;
	MaxStamina= LoadGameInstance->CharacterStats.MaxStamina;
	Coins= LoadGameInstance->CharacterStats.Coins;

	//装备武器
	if (WeaponStorage) {
		AItemStorage * Weapons=GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if (Weapons) {
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
			if (Weapons->WeaponMap.Contains(WeaponName) && WeaponName!=TEXT("")) { //判断是否含有当前关键字的值
				AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
				WeaponToEquip->Equip(this);//让玩家装备武器
			}
		}
	}
	if (World) {
		FName CurrentLevelName(*Map);//相当于解引用这样的 将FString转化为FName
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

	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));//把存档实例加载进来

	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Stamina = LoadGameInstance->CharacterStats.Stamina;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
	Coins = LoadGameInstance->CharacterStats.Coins;

	//装备武器
	if (WeaponStorage) {
		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if (Weapons) {
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
			if (Weapons->WeaponMap.Contains(WeaponName) && WeaponName != TEXT("")) { //判断是否含有当前关键字的值
				AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
				WeaponToEquip->Equip(this);//让玩家装备武器
			}
		}
	}



	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
}
void AMain::Empty() {
	
}