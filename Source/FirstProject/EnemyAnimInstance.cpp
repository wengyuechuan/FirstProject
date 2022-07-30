// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include"Enemy.h"
void UEnemyAnimInstance::NativeInitializeAnimation() {
	if (Pawn == nullptr) {
		//这里就获得了拥有当前动画对象的Pawn
		Pawn = TryGetPawnOwner();
		if (Pawn) { //进行类型转换得到我们需要的对象
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties() {
	if (Pawn == nullptr) {
		//这里就获得了拥有当前动画对象的Pawn
		Pawn = TryGetPawnOwner();
		if (Pawn) { //进行类型转换得到我们需要的对象
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
	if (Pawn) {
		//当前帧的矢量，也就是水平方向运动（行走）或者是垂直方向运动（跳跃）
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);//水平和
		MovementSpeed = LateralSpeed.Size();// 得到矢量的大小
	}
}