// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include"Components/SkeletalMeshComponent.h"
#include"Main.h"
#include"Engine/SkeletalMeshSocket.h"
#include"Sound/SoundCue.h"
#include"Kismet/GameplayStatics.h"
#include"Particles/ParticleSystemComponent.h"
#include"Components/BoxComponent.h"
#include"Enemy.h"
#include"Engine/SkeletalMeshSocket.h"
#include"Sound/SoundCue.h"

AWeapon::AWeapon() {
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());
	//盒体碰撞的预设


	bWeaponParticle = false;
	WeaponState = EWeaponState::EWS_Pickup;
	Damage = 25.f;
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);//碰撞不起效果
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //对于Pawn可以有Overlap事件
}
void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor &&(GetWeaponState()==EWeaponState::EWS_Pickup)) { //如果处于可以拾取的状态，那么就捡起来
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			//设置物体的重合效果
			Main->SetActiveOverlappingItem(this);
		}
	}
}
void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			//设置物体没有重合
			Main->SetActiveOverlappingItem(nullptr);
		}
	}
}
void AWeapon::Equip(AMain* Char) {
	if (Char) {
		SetInstigator(Char->GetController());
		//让摄像机忽略当前的物体
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		//让Pawn也忽略当前的物体
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		//停止模拟物理
		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket"); //得到了Socket，然后可以插入武器
		if (RightHandSocket) {
			//把物体插入到Socket中
			RightHandSocket->AttachActor(this,Char->GetMesh());
			this->bRotate = false;
			//先将原本的物体删除，然后获得新的物体
			//Char->GetEquipperdWeapon()->Destroy();//删除原本的武器 后面将一个值赋值给了空指针，然后去世了就
			Char->SetEquippedWeapon(this);
			Char->SetActiveOverlappingItem(nullptr);//设置没有重叠的物体
		}
		if (OnEquipSound) UGameplayStatics::PlaySound2D(this, OnEquipSound);
		//如果此时是false则关闭粒子特效，即装备之后没有粒子特效
		if (!bWeaponParticle) {
			IdleParticleComponent->Deactivate();//关闭粒子特效
		}
	}
}
void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//武器攻击
	if (OtherActor) {
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy) {
			if (Enemy->HitParticles) {
				const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("WeaponSocket");
				if (WeaponSocket) {
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh); 
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, SocketLocation, FRotator(0.f), false);//不开启自动销毁
				}
			}
			if(Enemy->HitSound){
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound);//挥动的声音
			}
			if (DamageTypeClass) {
				UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
			}
		}
	}
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}
void AWeapon::ActivateCollision() {
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AWeapon::DeactivateCollision() {
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}