// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include"Enemy.h"
void UEnemyAnimInstance::NativeInitializeAnimation() {
	if (Pawn == nullptr) {
		//����ͻ����ӵ�е�ǰ���������Pawn
		Pawn = TryGetPawnOwner();
		if (Pawn) { //��������ת���õ�������Ҫ�Ķ���
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties() {
	if (Pawn == nullptr) {
		//����ͻ����ӵ�е�ǰ���������Pawn
		Pawn = TryGetPawnOwner();
		if (Pawn) { //��������ת���õ�������Ҫ�Ķ���
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
	if (Pawn) {
		//��ǰ֡��ʸ����Ҳ����ˮƽ�����˶������ߣ������Ǵ�ֱ�����˶�����Ծ��
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);//ˮƽ��
		MovementSpeed = LateralSpeed.Size();// �õ�ʸ���Ĵ�С
	}
}