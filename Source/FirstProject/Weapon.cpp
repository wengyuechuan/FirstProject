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
	//������ײ��Ԥ��


	bWeaponParticle = false;
	WeaponState = EWeaponState::EWS_Pickup;
	Damage = 25.f;
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);//��ײ����Ч��
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //����Pawn������Overlap�¼�
}
void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor &&(GetWeaponState()==EWeaponState::EWS_Pickup)) { //������ڿ���ʰȡ��״̬����ô�ͼ�����
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			//����������غ�Ч��
			Main->SetActiveOverlappingItem(this);
		}
	}
}
void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			//��������û���غ�
			Main->SetActiveOverlappingItem(nullptr);
		}
	}
}
void AWeapon::Equip(AMain* Char) {
	if (Char) {
		SetInstigator(Char->GetController());
		//����������Ե�ǰ������
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		//��PawnҲ���Ե�ǰ������
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		//ֹͣģ������
		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket"); //�õ���Socket��Ȼ����Բ�������
		if (RightHandSocket) {
			//��������뵽Socket��
			RightHandSocket->AttachActor(this,Char->GetMesh());
			this->bRotate = false;
			//�Ƚ�ԭ��������ɾ����Ȼ�����µ�����
			//Char->GetEquipperdWeapon()->Destroy();//ɾ��ԭ�������� ���潫һ��ֵ��ֵ���˿�ָ�룬Ȼ��ȥ���˾�
			Char->SetEquippedWeapon(this);
			Char->SetActiveOverlappingItem(nullptr);//����û���ص�������
		}
		if (OnEquipSound) UGameplayStatics::PlaySound2D(this, OnEquipSound);
		//�����ʱ��false��ر�������Ч����װ��֮��û��������Ч
		if (!bWeaponParticle) {
			IdleParticleComponent->Deactivate();//�ر�������Ч
		}
	}
}
void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//��������
	if (OtherActor) {
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy) {
			if (Enemy->HitParticles) {
				const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("WeaponSocket");
				if (WeaponSocket) {
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh); 
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, SocketLocation, FRotator(0.f), false);//�������Զ�����
				}
			}
			if(Enemy->HitSound){
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound);//�Ӷ�������
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