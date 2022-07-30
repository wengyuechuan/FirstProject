// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include"Main.h" //包含这个头文件，我们可以得到我们的角色类
#include"Kismet/GameplayStatics.h"
#include"Particles/ParticleSystemComponent.h"
#include"Engine/World.h"
#include"Sound/SoundCue.h" //需要导入这个库
#include"Enemy.h"
#include"Kismet/GameplayStatics.h"

AExplosive::AExplosive() {
	Damage = 15.f;
}
//先调用父级的同名函数，然后再进行自己的操作
void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp,  OtherBodyIndex,  bFromSweep,  SweepResult);
	//OtherActor是与当前物体进行碰撞重合的物体
	if (OtherActor) {
		//类型的转换
		//如果OtherActor不是一个AMain类型的，那么就会返回NULL
		AMain* Main = Cast<AMain>(OtherActor);
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Main || Enemy) {
			if (OverlapParticles) {
				//这里的第二个参数的类型是UParticlesSystem，所以我们创建类型的时候要使用UParticlesSystem
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}
			if (OverlapSound) {
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}
			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);
			Destroy(); //强制垃圾回收
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent,  OtherActor, OtherComp, OtherBodyIndex);
}