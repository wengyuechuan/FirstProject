// Fill out your copyright notice in the Description page of Project Settings.


#include "Critter.h"
#include"Components/StaticMeshComponent.h" //这个其实没有必要引入，因为UE4已经逐步优化了项目结构，现在其实是可以直接调用的
#include"Camera/CameraComponent.h"//这个是必须要引入的
// Sets default values
ACritter::ACritter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	//init RootComponent 场景组件  ？？ 这里还是需要深入理解
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));

	//这里将静态网格组件附加到了场景组件下面了
	MeshComponent->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//这个就需要手动添加这个头文件了
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(-300.f, 0.f, 300.f));
	Camera->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	//设置摄像头默认观看着我们的Pawn，然后我们可以对其执行一定的操作
	// 定义之后不会移动
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	CurrentVelocity = FVector(0.f);
	//最大的速度
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
	SetActorLocation(NewLocation);//转换到新的位置
}

// Called to bind functionality to input
//用户输入的信息由这个PlayerInputComponent来记录
void ACritter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//开始绑定控制器
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this,&ACritter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACritter::MoveRight);

}

void ACritter::MoveForward(float Value) {

	CurrentVelocity.X = FMath::Clamp(Value, -1.f, 1.f)*maxspeed;//确保数值在这个范围内 如果超出则会压缩回当前的位置

}

void ACritter::MoveRight(float Value) {
	CurrentVelocity.Y = FMath::Clamp(Value, -1.f, 1.f) * maxspeed;//确保数值在这个范围内 如果超出则会压缩回当前的位置

}