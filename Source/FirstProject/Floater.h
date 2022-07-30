// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloater : public AActor //Actor����Ҫ����Blueprintable�ˣ���Ϊ�Ѿ��̳������������
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloater();
	
	//��UE�п���ʹ�õ���ѧ����
	/*
	* ���ã�Abs sin cos rand�� ���Ǻ����������������˶���һ���
	*/

	//�����Ǵ����ľ�̬�������
	//����������һ�������壬����˵�Ǹ����UE�����Ƥ��һ��Ķ���
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "ActorMeshComponents")
		UStaticMeshComponent* StaticMesh;    //����Ϸ�����������ǻ����������ͣ�������ʹ��ָ��������

		/*��һ������EditInstanceOnly����ñ���ֻ����Floater��ʵ���²ſ��Խ��в���
		* �����ǻ������C++�ഴ��һ����ͼ���ʱ�򣬲�����Ĭ����������������������ͬʱҲ�޷������������
		* �������ǵĹؿ��༭���������ʵ�������ʱ��ſ����޸�
		*/
		//Location used by SetActorLocation() when BeginPlay() is called
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
		FVector initialLocation;


	// Location of the Actor when dragged in from the editor
	//ǰ����ж�VisibleInstanceOnly��ζ�Ų�ϣ����ʵ���ж�����и��ģ�����ֻ�ܼ������ܽ��в���
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
		FVector PlacedLocation;


	/*
	* VisiableDefaultOnly��ֻ������ͼ������в鿴�������ܱ༭
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Floater Variables")
		FVector WorldOrigin;

	/*
	* �������κεط����б༭
	* ע�������ƴд������tmɵ��
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	FVector InitialDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	bool bShouldFloat; //���ڱ�־�����Ƿ���Ը�����ת
	/*
	* EditDefaultOnly����������ͼ������б༭
	* ������Ĭ��ģʽ�±༭
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Floater Variables")
	bool bIntializeFloaterLocations;

	/*
	* ���������ó�Ϊ�����޸ĵı���������������ͼ�ν�����в���
	*/
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="Floater Variables")
	FVector InitialForce;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector InitialTorque ;

private://private���治����EditAnywhere�� ��ʵ�ǿ����еģ����ﱨ���ԭ�� �����ܽ��ʱ��ǵ��Լ���������
	float Runningtime;
	//������Կ���UPROPERTY������һ���÷�����������Wave Parameters������Ч��������������Ϊ�˸ı����Ҳ����������Ҳ����õ�
	UPROPERTY(EditAnywhere,Category="FloaterVariables | Wave Parameters")
	float Amplitude;
	UPROPERTY(EditAnywhere, Category = "FloaterVariables | Wave Parameters")
	float TimeStretch;

	float BaseZLocation;
public:
	//Amplitude - how much we oscillate up and down
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Floater Variables")
	float A;

	//Period is 2*pi/(absolute value of B)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float B;

	//Phase Shift is (C/B)  ��λƫ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float C;

	//Vertical Shift is D
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float D;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
