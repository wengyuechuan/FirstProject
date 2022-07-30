// Fill out your copyright notice in the Description page of Project Settings.


#include "Critter.h"
#include"Components/StaticMeshComponent.h" //�����ʵû�б�Ҫ���룬��ΪUE4�Ѿ����Ż�����Ŀ�ṹ��������ʵ�ǿ���ֱ�ӵ��õ�
#include"Camera/CameraComponent.h"//����Ǳ���Ҫ�����
// Sets default values
ACritter::ACritter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	//init RootComponent �������  ���� ���ﻹ����Ҫ�������
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));

	//���ｫ��̬����������ӵ��˳������������
	MeshComponent->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//�������Ҫ�ֶ�������ͷ�ļ���
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(-300.f, 0.f, 300.f));
	Camera->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	//��������ͷĬ�Ϲۿ������ǵ�Pawn��Ȼ�����ǿ��Զ���ִ��һ���Ĳ���
	// ����֮�󲻻��ƶ�
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	CurrentVelocity = FVector(0.f);
	//�����ٶ�
	maxspeed = 100.f;
}

// Called when the game starts or when spawned
void ACritter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACritter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
	SetActorLocation(NewLocation);//ת�����µ�λ��
}

// Called to bind functionality to input
//�û��������Ϣ�����PlayerInputComponent����¼
void ACritter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//��ʼ�󶨿�����
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this,&ACritter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACritter::MoveRight);

}

void ACritter::MoveForward(float Value) {

	CurrentVelocity.X = FMath::Clamp(Value, -1.f, 1.f)*maxspeed;//ȷ����ֵ�������Χ�� ����������ѹ���ص�ǰ��λ��

}

void ACritter::MoveRight(float Value) {
	CurrentVelocity.Y = FMath::Clamp(Value, -1.f, 1.f) * maxspeed;//ȷ����ֵ�������Χ�� ����������ѹ���ص�ǰ��λ��

}