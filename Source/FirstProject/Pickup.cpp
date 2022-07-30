// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include"Main.h"
#include"Kismet/GameplayStatics.h"
#include"Particles/ParticleSystemComponent.h"
#include"Engine/World.h"
#include"Sound/SoundCue.h" //��Ҫ���������
APickup::APickup() {
	//CoinCount = 1;
}
void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor) {
		//���͵�ת��
		//���OtherActor����һ��AMain���͵ģ���ô�ͻ᷵��NULL
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			OnPickupBP(Main);
			//Main->IncrementCoins(CoinCount);
			Main->PickupLocations.Add(GetActorLocation());
			if (OverlapParticles) {
				//����ĵڶ���������������UParticlesSystem���������Ǵ������͵�ʱ��Ҫʹ��UParticlesSystem
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}
			if (OverlapSound) {
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			Destroy(); //ǿ����������
		}
	}
}

void APickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}