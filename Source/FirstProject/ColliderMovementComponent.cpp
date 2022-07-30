// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

	//���ø�����ͬ����������֤���ܶ������˳���
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);

	//PawnOwner      Pawn that owns this component
	//UpdatedComponent       The component we move and update.
	//ShouleSkipUpdate        Possibly skip update if moved component is not rendered or can't move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}

	/*
	* ConsumeInputVector() �᷵�������ڿ����������õ��ƶ��ķ���Ȼ��ԭ����Vector���ã�Ȼ�����GetClampedToMaxSize,��Clamp����˼��࣬���ǿ��Խ���ѹ������˼
	*/
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f);

	if (!DesiredMovementThisFrame.IsNearlyZero()) { //�ж��ƶ��ľ����Ƿ�������0
		FHitResult Hit; //����Ƿ����ײ�� ������ײ��������ĺ����Ὣ��ײ����Ϣ���ݸ�Hit
		//Ȼ����ȷ���ƶ�����
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(),true, Hit);
		/*
		* ����Ƿ�ײ��
		*	�����ײ����֮������ǽ�ƶ�
		*/
		if (Hit.IsValidBlockingHit()) {
			//��һ����������ײʱ��ʸ������
			//��ײʱ�� һ�����1-hit.Time
			//������Ϊ��ײ��ķ���ʸ��
			//���ĸ�Ϊ��ײ����
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
			UE_LOG(LogTemp, Warning, TEXT("Valid Blocking HiT"));
		}
	}
}