// Fill out your copyright notice in the Description page of Project Settings.

/*
* 最后bug修复，当我们在第一关如果不存档就直接回档，就会闪退，这里的问题我打算接着使用gameinstance来解决
*/
#include "Enemy.h"
#include"Components/SphereComponent.h"
#include"AIController.h"
#include"Main.h"
#include "Kismet/KismetSystemLibrary.h"
#include"Components/BoxComponent.h"
#include"Sound/SoundCue.h"
#include"Kismet/GameplayStatics.h"
#include"Engine/SkeletalMeshSocket.h"
#include"Animation/AnimInstance.h"
#include"TimerManager.h"
#include"Components/CapsuleComponent.h"
#include"MainPlayerController.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));

	//将盒子模型附加在骨骼的socket上面 这个函数还是不太明白，需要深入学习一下子 第二个参数捕捉到了顶部
	//其实在看了蓝图界面之后，也逐渐明白了一切，这个其实就是绑定在骨骼的socket上面的盒子，和这一块骨骼一起运动
	CombatCollision->SetupAttachment(GetMesh(),FName("EnemySocket"));
	//CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));


	bOverlappingCombatSphere = false;

	Health = 75.f;
	MaxHealth = 100.f;
	Damage = 10.f;
	bAttacking = false;
	//事件间隔 这里3.5有点长昂
	AttackMinTime = 0.5f;
	AttackMaxTime = 2.f;

	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);//默认为静止状态
	DeathDelay = 3.f;
	bHasValidTarget = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();
	AIController = Cast<AAIController>(GetController());
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);
	//同样的，为了让敌人的攻击和
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);//碰撞不起效果
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //对于Pawn可以有Overlap事件

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//和检测敌人的区域重合
void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor && Alive()) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			//CombatTarget = Main;
			MoveToTarget(Main);
		}
	}
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			bHasValidTarget = false;
			if (Main->CombatTarget == this) { // 与当前重合的情况则删除
				Main->SetCombatTarget(nullptr);
			}
			Main->SetHasCombatTarget(false);
			Main->UpdateCombatTarget();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);//进入常态
			if (AIController) { //此时让其停止运动
				AIController->StopMovement();
			}
		}
	}
}

//和检测敌人的区域重合
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor && Alive()) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			bHasValidTarget = true;
			Main->SetCombatTarget(this);//设置当前的攻击对象为敌人
			Main->SetHasCombatTarget(true);
			Main->UpdateCombatTarget();
			CombatTarget = Main;
			bOverlappingCombatSphere = true;
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor && OtherComp ) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			bOverlappingCombatSphere = false;
			MoveToTarget(Main);
			CombatTarget = nullptr;
			if (Main->CombatTarget == this) {
				Main->SetCombatTarget(nullptr);
				Main->bHasCombatTarget = false;
				Main->UpdateCombatTarget();
			}
			if (Main->MainPlayerController) {
				USkeletalMeshComponent* MainMesh = Cast<USkeletalMeshComponent>(OtherActor);
				if (MainMesh) Main->MainPlayerController->RemoveEnemyHealthBar();
			}
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}
void AEnemy::MoveToTarget(AMain* Target) {
	//向物体运动
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController) {
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		//这个值的设定，其实是用来判断当前寻路到玩家之后两个胶囊体之间的距离
		MoveRequest.SetAcceptanceRadius(20.f);
		FNavPathSharedPtr  NavPath;//Out为输出信息，所以开始的时候不需要进行初始化也可以解决这个问题
		AIController->MoveTo(MoveRequest,&NavPath);


		//调试使用的
		//for (auto Point : PathPoints) {
		//	FVector Location = Point.Location;

		//	//进行调试 打印出路径
		//	UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, 1.5f);
		//}
	}
}
void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//武器攻击
	if (OtherActor && Alive()) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			if (Main->HitParticles) {
				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
				if (TipSocket) {
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, SocketLocation, FRotator(0.f), false);//不开启自动销毁
				}
			}
			if (Main->HitSound) {
				UGameplayStatics::PlaySound2D(this, Main->HitSound);//挥动的声音
			}
			if (DamageTypeClass) {
				//最后一个参数的作用是伤害的机制
				//如果不进行创建则是默认的伤害类型
				UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}
/*
* 使用UMG以及使用HUD的时候，这里需要注意一个问题，我们需要在FirstProject.Build.cs中追加内容，否则会出现编译失败然后无法识别的问题
* 然后使用AIController的时候，一定要增加寻路网格体才行
*/
void AEnemy::ActivateCollision() {
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AEnemy::DeactivateCollision() {
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AEnemy::AttackEnd() {
	UE_LOG(LogTemp, Warning, TEXT("attack"));
	bAttacking = false;
	if (bOverlappingCombatSphere) {
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	}
}
void AEnemy::Attack() {
	if (Alive() && bHasValidTarget) {
		if (AIController) {
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking); //处于攻击状态
		}
		if (!bAttacking) {
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance) {
				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
			}
			if (SwingSound) {
				UGameplayStatics::PlaySound2D(this, SwingSound);
			}
		}
	}
}
float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	if (Health - DamageAmount <= 0.f) {
		Health -=DamageAmount;
		Die(DamageCauser);
	}
	else {
		Health -= DamageAmount;
	}
	return DamageAmount;
}
void AEnemy::Die(AActor * Causer) {
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);//死亡状态
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Death"),CombatMontage);
	}
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bAttacking = false;

	AMain* Main = Cast<AMain>(Causer);
	if (Main) {
		Main->UpdateCombatTarget();
	}
}
void AEnemy::DeathEnd(){
	GetMesh()->bPauseAnims = true;//画面暂停
	GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear,DeathDelay);//调用删除函数
}
bool AEnemy::Alive() {
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}
void AEnemy::Disappear() {
	Destroy();
}