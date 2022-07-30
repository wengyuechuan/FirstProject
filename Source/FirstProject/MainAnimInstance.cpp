// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"Main.h"
void UMainAnimInstance::NativeInitializeAnimation() {
	if (Pawn == nullptr) {
		//这里就获得了拥有当前动画对象的Pawn
		Pawn = TryGetPawnOwner(); 
		if (Pawn) {
			Main = Cast<AMain>(Pawn);
		}
	}
}
/// <summary>
/// 相当于Tick函数，每一帧要做什么
/// </summary>
void UMainAnimInstance::UpdateAnimationProperties() {
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
	}

	if (Pawn) {
		//当前帧的矢量，也就是水平方向运动（行走）或者是垂直方向运动（跳跃）
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);//水平和

		MovementSpeed = LateralSpeed.Size();// 得到矢量的大小
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();//每一帧更新是否是在跳跃的状态
		if (Main == nullptr) {
			Main = Cast<AMain>(Pawn);
		}
	}
}