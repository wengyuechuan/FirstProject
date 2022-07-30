// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include"Components/SphereComponent.h" //这里逐步掌握了进行引入库的方法
#include"Components/InputComponent.h"
#include"GameFramework/SpringArmComponent.h"
#include"Camera/CameraComponent.h"
#include"GameFramework/MovementComponent.h"
#include"ColliderMovementComponent.h"
// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//SphereComponent的作用是用于处理碰撞等一类动作，那么我们让它作为新的根 上面的设置就可以删除掉了
	//利用函数和利用变量设置根组件都是很好的方式，UE这里提供了多种方法
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->InitSphereRadius(40.0f);//设置碰撞球体的直径
	//设置碰撞体的档案名称，这个作用是为了碰撞通道获取时的名称
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	/// <summary>
	/// 这里有一个更好的理解，就是说这个网格才是当前物体的视觉展示，当然还可以具有一些物理特性
	/// </summary>
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	//在代码中可以对我们创建的一些属性进行修改，但是一般是在编辑器的图形界面里面修改
	//但是下面的方法可以进行一定的修改，我们下面创建一个ConstructorHelpers对象，然后通过这个对象，找到我们的静态网格资源，然后访问
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshComponentAsset.Succeeded()) { //访问资源成功
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);//将当前的静态网格设置成为我们选择的这个网格
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f)); //改变位置（位置）
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f)); //设置相对比例（缩放）
	}
	//SpringArm是弹簧臂，这个后面会知道具体的作用是什么，弹簧臂组件的作用可以根据Gameplay的情况进行拓展和回缩，也就是相当于一个支架拿着摄像机，然后可以让他放大缩小这样的功能
	//这个部分可以读文档 https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/UsingCameras/SpringArmComponents/
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	//第二个参数是包含在类里面的
	Camera->SetupAttachment(ACollider::GetSpringArmComponent(),USpringArmComponent::SocketName);

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("OurMovementComponent"));
	OurMovementComponent->UpdatedComponent = GetRootComponent();//这个变量的作用是确定这个MovementComponent控制的是谁，也就是说

	CameraInput = FVector2D(0.f, 0.f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator NewRotation = GetActorRotation();

	NewRotation.Yaw += CameraInput.X;//旋转摄像机 来改变Yaw
	SetActorRotation(NewRotation);

	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += CameraInput.Y, -80.f, -15.f);

	SpringArm->SetWorldRotation(NewSpringArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//键盘按键的绑定
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this,&ACollider::MoveRight);

	//鼠标的移动
	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::YawCamera);
}
/*
* 物体仍然无法运动的原因：
*	Pawn不会自己创建MovementComponent，而需要我们自定义这个变量，注意Character会自动创建，现在我们在练习使用这个代码，加深理解
*	UPawnMovementComponent* MovementComponent = GetMovementComponent();
*	上面这个语句是AddMovementInput的第一行，也就是获得当前的MovementComponent，但是我们当前类没有啊，那只能是接收到一个False，也就是空，
*	那这样也就没办法执行了，这也就是为什么我们自己创建的物体不能跟随键盘按键进行运动
*/
void ACollider::MoveForward(float Value) {
	FVector Forward = GetActorForwardVector();
	if (OurMovementComponent) {
		OurMovementComponent->AddInputVector(Value * Forward);
	}

}
void ACollider::MoveRight(float Value) {
	FVector Right = GetActorRightVector();
	if (OurMovementComponent) {
		OurMovementComponent->AddInputVector(Value *Right);
	}
}

//函数的重载，重写以及重定义都需要重新复习一下，这里的内容还是很乱
UPawnMovementComponent* ACollider::GetMovementComponent() const{
	return OurMovementComponent;
}

void ACollider::YawCamera(float AxisValue) {
	CameraInput.X = AxisValue;
}
void ACollider::PitchCamera(float AxisValue) {
	CameraInput.Y = AxisValue;
}
