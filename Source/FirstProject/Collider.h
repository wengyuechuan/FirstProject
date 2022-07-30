// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

UCLASS()
class FIRSTPROJECT_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere,Category="Mesh")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere,Category = "Mesh")
	class USphereComponent* SphereComponent; //����ģ������

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USpringArmComponent* SpringArm;//���ڿ�����������ƶ�

	/*
	* ��������£����Զ�����MovementComponent��������Ҫ���ǽ��д���
	*/
	UPROPERTY(VisibleAnywhere,Category="Movement")
	class UColliderMovementComponent* OurMovementComponent; 

	/*
	* ������Ҫ��дGetMovementComponent
	*/
	virtual UPawnMovementComponent* GetMovementComponent() const override;


	//����ʹ����FORCEINLINE��������Ϊ�������������Ϊ��������ʹ�ã�����ִ�е�Ч�� �൱�ڽ�����ת����һ����
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }

	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* Mesh) { MeshComponent = Mesh; }

	FORCEINLINE USphereComponent* GetSphereComponent() { return SphereComponent; }

	FORCEINLINE void SetSphereComponent(USphereComponent* Sphere) { SphereComponent = Sphere; }

	FORCEINLINE UCameraComponent* GetCameraComponent() { return Camera; }

	FORCEINLINE void GetCameraComponent(UCameraComponent* camera) { Camera = camera; }
	
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* springarm) { SpringArm=springarm; }
	
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return SpringArm; }

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	FVector2D  CameraInput;
};
