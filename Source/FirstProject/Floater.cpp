// Fill out your copyright notice in the Description page of Project Settings.


#include"Floater.h"
#include"Components/StaticMeshComponent.h" //引入当前的文件 对静态网格进行一些操作
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
	Super::BeginPlay();//游戏开始时会调用所有Actor对象的BeginPlay()方法
	//利用FRand生成的随机数，只在0-1之间
	//可以利用FRandRange来进行
	float InitialX = FMath::FRandRange(-100.f,100.f);
	float InitialY = FMath::FRandRange(-100.f,100.f);
	float InitialZ = FMath::FRandRange(-100.f,100.f);

	initialLocation.X = InitialX;
	initialLocation.Y = InitialY;
	initialLocation.Z = InitialZ;

	initialLocation *= 20.f; //这里其实是因为FVector重写了*，这里其实是每一个数值都乘20得到一个新的FVector
	

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
	// 这里主要讲述的就是这个函数其实是局部的坐标系，而不是以整个地图的坐标系进行规定的
	//AddActorLocalOffset(LocalOffset, true, &HitResult);
	//下面整个函数是以整个地图的坐标系来规定的

	//AddActorWorldOffset(LocalOffset, true, &HitResult);

	///*
	//* 旋转Rotation
	//* 分别为绕着y，z，x轴旋转
	//*/
	//FRotator Rotation = FRotator(30.0f, 0.0f, 0.0f);
	//AddActorLocalRotation(Rotation);

	/*
	* 关于力的使用
	*/
	//静态网格的操作需要引入一个新的头文件 静态网格可以接收力 碰撞等操作
	//第二个参数为骨骼名称
	//这里是给其一个初始的力
	// 此时FVector为力矩
	//StaticMesh->AddForce(InitialForce);

	///*
	//* 这里是扭矩
	//* 还是利用FVector
	//* 是一个用于旋转的力
	//*/
	//StaticMesh->AddTorque(InitialTorque);

}

// Called every frame
// DeltaTime是自上一帧以后以秒为单位的时间量
void AFloater::Tick(float DeltaTime)
{
	/*
	* 第三天从AddActorLocalOffset开始
	*/
	Super::Tick(DeltaTime);
	if (bShouldFloat) {
		///*
		//* HitResult：标识某一碰撞命中之后会发生的结果，存储了碰撞时的一些信息
		//*/
		//FHitResult HitResult;
		////第一个参数为每一帧改变的方向，第二个参数为一个bool型变量的值为bSweep 不要执行扫除的操作 也就是可以穿越网格，如果我们设置为true则可以穿越网格，第三个参数为FHitResult
		//AddActorLocalOffset(InitialDirection,true,&HitResult); 
		//FVector HitLocation = HitResult.Location;


		//int a = 1;
		//UE_LOG(LogTemp, Warning, TEXT("Hit Location: X=%f,Y=%f,Z=%f"), HitLocation.X, HitLocation.Y, HitLocation.Z);
		

		/*
		* 我们在下面会使用正弦和余弦函数
		*/

		FVector NewLocation=GetActorLocation();//获得当前Actor的位置
		//这里是以正弦函数为格式进行操作的，我们可以看到括号里面乘一个数字，会改变上下摆动的速度，括号外面乘一个数字，会改变幅度
		//这里我们让X为正弦函数，Y为余弦函数，这样就会在X-Y平面上产生一个圆周运动
		//NewLocation.X = NewLocation.X + Amplitude*FMath::Sin(TimeStretch*Runningtime);
		//NewLocation.Y = NewLocation.Y + Amplitude * FMath::Cos(TimeStretch * Runningtime);
		NewLocation.Z = BaseZLocation + A * FMath::Sin(B *Runningtime+C)+D; // Period=2*PI/ABS(B)
		SetActorLocation(NewLocation); //设置物体当前的位置
		Runningtime += DeltaTime; //这样就可以记录运行的总时间
		//当我们拥有了这个Runningtime这个变量之后，由于其连续性，我们可以得到一个连续的正弦曲线
	}



	//FRotator Rotation = FRotator(0.0f, 0.0f, 1.0f);
	////会围绕局部的x轴来进行旋转，而不是世界的x轴
	//AddActorWorldRotation(Rotation);
}

