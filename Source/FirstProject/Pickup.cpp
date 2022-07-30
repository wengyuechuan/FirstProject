// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include"Main.h"
#include"Kismet/GameplayStatics.h"
#include"Particles/ParticleSystemComponent.h"
#include"Engine/World.h"
#include"Sound/SoundCue.h" //需要导入这个库
APickup::APickup() {
	//CoinCount = 1;
}
void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor) {
		//类型的转换
		//如果OtherActor不是一个AMain类型的，那么就会返回NULL
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			OnPickupBP(Main);
			//Main->IncrementCoins(CoinCount);
			Main->PickupLocations.Add(GetActorLocation());
			if (OverlapParticles) {
				//这里的第二个参数的类型是UParticlesSystem，所以我们创建类型的时候要使用UParticlesSystem
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}
			if (OverlapSound) {
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			Destroy(); //强制垃圾回收
		}
	}
}

void APickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}