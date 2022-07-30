// Fill out your copyright notice in the Description page of Project Settings.


#include"Floater.h"
#include"Components/StaticMeshComponent.h" //���뵱ǰ���ļ� �Ծ�̬�������һЩ����
// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));
	initialLocation = FVector(0.0f, 0.0f, 0.0f);
	PlacedLocation = FVector(0.0f);
	WorldOrigin = FVector(0.0f, 0.0f, 0.0f);
	InitialDirection = FVector(0.0f, 0.0f, 0.0f);
	bShouldFloat = false;
	bIntializeFloaterLocations = false;
	InitialForce = FVector(20000.f, 0.0f, 0.0f);
	InitialTorque = FVector(20000.f, 0.0f, 0.0f);
	A = 0.f;
	B = 0.f;
	C = 0.f;
	D = 0.f;
	Runningtime = 0;
	Amplitude = 1.f;
	TimeStretch = 1.f;
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();//��Ϸ��ʼʱ���������Actor�����BeginPlay()����
	//����FRand���ɵ��������ֻ��0-1֮��
	//��������FRandRange������
	float InitialX = FMath::FRandRange(-100.f,100.f);
	float InitialY = FMath::FRandRange(-100.f,100.f);
	float InitialZ = FMath::FRandRange(-100.f,100.f);

	initialLocation.X = InitialX;
	initialLocation.Y = InitialY;
	initialLocation.Z = InitialZ;

	initialLocation *= 20.f; //������ʵ����ΪFVector��д��*��������ʵ��ÿһ����ֵ����20�õ�һ���µ�FVector
	

	PlacedLocation = GetActorLocation();
	/*
	* when the variable is true,the operation takes up
	*/
	if (bIntializeFloaterLocations) {
		SetActorLocation(initialLocation);
	}
	BaseZLocation = PlacedLocation.Z;

	//FHitResult HitResult;
	//FVector LocalOffset = FVector(200.f, 300.f, 400.f);
	// ������Ҫ�����ľ������������ʵ�Ǿֲ�������ϵ����������������ͼ������ϵ���й涨��
	//AddActorLocalOffset(LocalOffset, true, &HitResult);
	//����������������������ͼ������ϵ���涨��

	//AddActorWorldOffset(LocalOffset, true, &HitResult);

	///*
	//* ��תRotation
	//* �ֱ�Ϊ����y��z��x����ת
	//*/
	//FRotator Rotation = FRotator(30.0f, 0.0f, 0.0f);
	//AddActorLocalRotation(Rotation);

	/*
	* ��������ʹ��
	*/
	//��̬����Ĳ�����Ҫ����һ���µ�ͷ�ļ� ��̬������Խ����� ��ײ�Ȳ���
	//�ڶ�������Ϊ��������
	//�����Ǹ���һ����ʼ����
	// ��ʱFVectorΪ����
	//StaticMesh->AddForce(InitialForce);

	///*
	//* ������Ť��
	//* ��������FVector
	//* ��һ��������ת����
	//*/
	//StaticMesh->AddTorque(InitialTorque);

}

// Called every frame
// DeltaTime������һ֡�Ժ�����Ϊ��λ��ʱ����
void AFloater::Tick(float DeltaTime)
{
	/*
	* �������AddActorLocalOffset��ʼ
	*/
	Super::Tick(DeltaTime);
	if (bShouldFloat) {
		///*
		//* HitResult����ʶĳһ��ײ����֮��ᷢ���Ľ�����洢����ײʱ��һЩ��Ϣ
		//*/
		//FHitResult HitResult;
		////��һ������Ϊÿһ֡�ı�ķ��򣬵ڶ�������Ϊһ��bool�ͱ�����ֵΪbSweep ��Ҫִ��ɨ���Ĳ��� Ҳ���ǿ��Դ�Խ���������������Ϊtrue����Դ�Խ���񣬵���������ΪFHitResult
		//AddActorLocalOffset(InitialDirection,true,&HitResult); 
		//FVector HitLocation = HitResult.Location;


		//int a = 1;
		//UE_LOG(LogTemp, Warning, TEXT("Hit Location: X=%f,Y=%f,Z=%f"), HitLocation.X, HitLocation.Y, HitLocation.Z);
		

		/*
		* �����������ʹ�����Һ����Һ���
		*/

		FVector NewLocation=GetActorLocation();//��õ�ǰActor��λ��
		//�����������Һ���Ϊ��ʽ���в����ģ����ǿ��Կ������������һ�����֣���ı����°ڶ����ٶȣ����������һ�����֣���ı����
		//����������XΪ���Һ�����YΪ���Һ����������ͻ���X-Yƽ���ϲ���һ��Բ���˶�
		//NewLocation.X = NewLocation.X + Amplitude*FMath::Sin(TimeStretch*Runningtime);
		//NewLocation.Y = NewLocation.Y + Amplitude * FMath::Cos(TimeStretch * Runningtime);
		NewLocation.Z = BaseZLocation + A * FMath::Sin(B *Runningtime+C)+D; // Period=2*PI/ABS(B)
		SetActorLocation(NewLocation); //�������嵱ǰ��λ��
		Runningtime += DeltaTime; //�����Ϳ��Լ�¼���е���ʱ��
		//������ӵ�������Runningtime�������֮�������������ԣ����ǿ��Եõ�һ����������������
	}



	//FRotator Rotation = FRotator(0.0f, 0.0f, 1.0f);
	////��Χ�ƾֲ���x����������ת�������������x��
	//AddActorWorldRotation(Rotation);
}

