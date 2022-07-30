// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include"Main.h" //�������ͷ�ļ������ǿ��Եõ����ǵĽ�ɫ��
#include"Kismet/GameplayStatics.h"
#include"Particles/ParticleSystemComponent.h"
#include"Engine/World.h"
#include"Sound/SoundCue.h" //��Ҫ���������
#include"Enemy.h"
#include"Kismet/GameplayStatics.h"

AExplosive::AExplosive() {
	Damage = 15.f;
}
//�ȵ��ø�����ͬ��������Ȼ���ٽ����Լ��Ĳ���
void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp,  OtherBodyIndex,  bFromSweep,  SweepResult);
	//OtherActor���뵱ǰ���������ײ�غϵ�����
	if (OtherActor) {
		//���͵�ת��
		//���OtherActor����һ��AMain���͵ģ���ô�ͻ᷵��NULL
		AMain* Main = Cast<AMain>(OtherActor);
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Main || Enemy) {
			if (OverlapParticles) {
				//����ĵڶ���������������UParticlesSystem���������Ǵ������͵�ʱ��Ҫʹ��UParticlesSystem
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}
			if (OverlapSound) {
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}
			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);
			Destroy(); //ǿ����������
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent,  OtherActor, OtherComp, OtherBodyIndex);
}