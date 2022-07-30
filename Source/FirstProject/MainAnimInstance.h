// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 这个是我们的动画类，非常好，终于进入到基本上是最后一步了，角色要动起来了
 */
UCLASS()
class FIRSTPROJECT_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable,Category=AnimationProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	float MovementSpeed;
	
	//When the character is in the air then the var is true
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsInAir;
	
	//The Pawn of Character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AMain* Main;
};

/*
*  需要进行的操作，创建一个对应的蓝图类，然后左上角重新设置其父类，然后这样可以使用蓝图本身的内容。
*  同时又可以使用其他的内容
* 全局运动(global motion)：字面意思就是整个画面或物体都发生了运动或改变，几乎和上一个时间段没有任何相同之处。举个栗子：把人比作此物体，global motion就像是你从躺下到站起来，人的整体全部发生了变化。
* 局部运动(local motion）：字面意思就是整个画面或物体只有小部分运动或改变，其余的大部分和上一个时间段的大部分场景还是相同的。举个栗子：把人比作此物体，local motion就像是你站在那里拿着栗子摆手一样，只有手的位置生了变化。
* 上面两个概念需要明确一下
* 
* 然后就是状态转换的问题，中间的状态转换条件，当变量开始改变成为true时，状态转换到jumpup，
* 然后就是float比较节点，Time Remaining剩余百分之八十的时候，播放下一个动画，就是下落动画
*/