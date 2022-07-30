// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

	//调用父级的同名函数，保证功能都调用了出来
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);

	//PawnOwner      Pawn that owns this component
	//UpdatedComponent       The component we move and update.
	//ShouleSkipUpdate        Possibly skip update if moved component is not rendered or can't move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}

	/*
	* ConsumeInputVector() 会返回我们在控制器中设置的移动的方向，然后将原本的Vector重置，然后就是GetClampedToMaxSize,跟Clamp的意思差不多，都是可以进行压缩的意思
	*/
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f);

	if (!DesiredMovementThisFrame.IsNearlyZero()) { //判断移动的距离是否趋近于0
		FHitResult Hit; //检测是否存在撞击 若出现撞击，下面的函数会将碰撞的信息传递给Hit
		//然后正确的移动物体
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(),true, Hit);
		/*
		* 检测是否被撞击
		*	如果被撞击了之后沿着墙移动
		*/
		if (Hit.IsValidBlockingHit()) {
			//第一个变量：碰撞时的矢量方向
			//碰撞时间 一般等于1-hit.Time
			//第三个为碰撞面的方向矢量
			//第四个为碰撞本身
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
			UE_LOG(LogTemp, Warning, TEXT("Valid Blocking HiT"));
		}
	}
}