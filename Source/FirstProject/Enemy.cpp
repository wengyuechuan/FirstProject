// Fill out your copyright notice in the Description page of Project Settings.

/*
* ���bug�޸����������ڵ�һ��������浵��ֱ�ӻص����ͻ����ˣ�����������Ҵ������ʹ��gameinstance�����
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

	//������ģ�͸����ڹ�����socket���� ����������ǲ�̫���ף���Ҫ����ѧϰһ���� �ڶ���������׽���˶���
	//��ʵ�ڿ�����ͼ����֮��Ҳ��������һ�У������ʵ���ǰ��ڹ�����socket����ĺ��ӣ�����һ�����һ���˶�
	CombatCollision->SetupAttachment(GetMesh(),FName("EnemySocket"));
	//CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));


	bOverlappingCombatSphere = false;

	Health = 75.f;
	MaxHealth = 100.f;
	Damage = 10.f;
	bAttacking = false;
	//�¼���� ����3.5�е㳤��
	AttackMinTime = 0.5f;
	AttackMaxTime = 2.f;

	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);//Ĭ��Ϊ��ֹ״̬
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
	//ͬ���ģ�Ϊ���õ��˵Ĺ�����
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);//��ײ����Ч��
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //����Pawn������Overlap�¼�

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

//�ͼ����˵������غ�
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
			if (Main->CombatTarget == this) { // �뵱ǰ�غϵ������ɾ��
				Main->SetCombatTarget(nullptr);
			}
			Main->SetHasCombatTarget(false);
			Main->UpdateCombatTarget();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);//���볣̬
			if (AIController) { //��ʱ����ֹͣ�˶�
				AIController->StopMovement();
			}
		}
	}
}

//�ͼ����˵������غ�
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor && Alive()) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			bHasValidTarget = true;
			Main->SetCombatTarget(this);//���õ�ǰ�Ĺ�������Ϊ����
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
	//�������˶�
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController) {
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		//���ֵ���趨����ʵ�������жϵ�ǰѰ·�����֮������������֮��ľ���
		MoveRequest.SetAcceptanceRadius(20.f);
		FNavPathSharedPtr  NavPath;//OutΪ�����Ϣ�����Կ�ʼ��ʱ����Ҫ���г�ʼ��Ҳ���Խ���������
		AIController->MoveTo(MoveRequest,&NavPath);


		//����ʹ�õ�
		//for (auto Point : PathPoints) {
		//	FVector Location = Point.Location;

		//	//���е��� ��ӡ��·��
		//	UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, 1.5f);
		//}
	}
}
void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//��������
	if (OtherActor && Alive()) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			if (Main->HitParticles) {
				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
				if (TipSocket) {
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, SocketLocation, FRotator(0.f), false);//�������Զ�����
				}
			}
			if (Main->HitSound) {
				UGameplayStatics::PlaySound2D(this, Main->HitSound);//�Ӷ�������
			}
			if (DamageTypeClass) {
				//���һ���������������˺��Ļ���
				//��������д�������Ĭ�ϵ��˺�����
				UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}
/*
* ʹ��UMG�Լ�ʹ��HUD��ʱ��������Ҫע��һ�����⣬������Ҫ��FirstProject.Build.cs��׷�����ݣ��������ֱ���ʧ��Ȼ���޷�ʶ�������
* Ȼ��ʹ��AIController��ʱ��һ��Ҫ����Ѱ·���������
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
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking); //���ڹ���״̬
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
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);//����״̬
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
	GetMesh()->bPauseAnims = true;//������ͣ
	GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear,DeathDelay);//����ɾ������
}
bool AEnemy::Alive() {
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}
void AEnemy::Disappear() {
	Destroy();
}