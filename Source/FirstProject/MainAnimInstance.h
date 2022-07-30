// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * ��������ǵĶ����࣬�ǳ��ã����ڽ��뵽�����������һ���ˣ���ɫҪ��������
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
*  ��Ҫ���еĲ���������һ����Ӧ����ͼ�࣬Ȼ�����Ͻ����������丸�࣬Ȼ����������ʹ����ͼ��������ݡ�
*  ͬʱ�ֿ���ʹ������������
* ȫ���˶�(global motion)��������˼����������������嶼�������˶���ı䣬��������һ��ʱ���û���κ���֮ͬ�����ٸ����ӣ����˱��������壬global motion������������µ�վ�������˵�����ȫ�������˱仯��
* �ֲ��˶�(local motion����������˼�����������������ֻ��С�����˶���ı䣬����Ĵ󲿷ֺ���һ��ʱ��εĴ󲿷ֳ���������ͬ�ġ��ٸ����ӣ����˱��������壬local motion��������վ�������������Ӱ���һ����ֻ���ֵ�λ�����˱仯��
* ��������������Ҫ��ȷһ��
* 
* Ȼ�����״̬ת�������⣬�м��״̬ת����������������ʼ�ı��Ϊtrueʱ��״̬ת����jumpup��
* Ȼ�����float�ȽϽڵ㣬Time Remainingʣ��ٷ�֮��ʮ��ʱ�򣬲�����һ���������������䶯��
*/