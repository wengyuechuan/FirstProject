// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include"Components/BoxComponent.h"
#include"Components/StaticMeshComponent.h"
// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	//TriggerBox不会移动，而由于门与其组成了整体，需要进行移动，这样的话，我们根组件就设置成TriggerBox就可以了
	//这里上面的理解可能不太全面，这里的理解其实是让其他的物体随着这个物体的动作而动起来，这样我们可以得到我们想要的结果
	RootComponent = TriggerBox;
	
	/*
	* 设置碰撞类型
	* QueryOnly是只与重叠相关 里面还有很多不同的值，这里在总结的时候再弄
	*/
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	/*
	* 这里的道理有那么一点点难懂，然后我查询了一篇比较好的文章，明白了这个东西
	* https://zhuanlan.zhihu.com/p/91669703
	* https://zhuanlan.zhihu.com/p/70045930?from_voters_page=true Gamemode机制，这些文章可以多读一读，然后梳理一下GamePlay的机制
	* 首先这个碰撞通道，可以理解为参与碰撞的物体的本身类型，分为
	* WorldStatic-游戏中的静态对象，即不会以任何方式移动，如地形等。
	* WorldDynamic-除了下面4种之外的游戏内可移动物体。
	* Pawn-由玩家或者AI控制的角色。
	* PhysicsBody-所有可以被Simulate的Physics Body或者Object。
	* Vehicle-标记为Vehicle后，玩家的Pawn可以进入其中。
	* Destructible-任何可破坏的Actor，可使用Destructible Mesh Editor进行编辑。
	* 而我们现在设计的开关这个设定，是不会以任何方式移动，所以我们选择了WorldStatic这个类型
	* 之后的内容可以完全参考文章，
	* 写的非常不错，当我完成我的第一个游戏项目之后，我将开始阅读更多的关于引擎底层的东西
	*/
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	//设置这里的碰撞的响应类型 先忽略所有的类型，然后设置特定的类型反应 也就是对玩家可操纵角色进行反应
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); 
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//设置形状
	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 32.f));

	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switch"));
	FloorSwitch->SetupAttachment(GetRootComponent());

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(TriggerBox);

	SwitchTime = 2.0f;
	bCharacterOnSwitch = false;
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
	//Overlap即重叠，就是其他物体与当前物体重叠（发生碰撞等）
	//AddDynamic的机制和C#的委托（delegate）差不多效果，可以将事件进行绑定，这里也是将事件进行绑定的含义
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);
	InitialDoorLocation = Door->GetComponentLocation();
	InitialSwitchLocation = FloorSwitch->GetComponentLocation();
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// <summary>
/// 绑定之后，当角色与机关重合，就会调用这个函数，并把一些信息（参数）传递到这个函数中来
/// </summary>
/// <param name="OverlappedComponent"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="OtherBodyIndex"></param>
/// <param name="bFromSweep"></param>
/// <param name="SweepResult"></param>
void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin."));
	if (!bCharacterOnSwitch) {
		bCharacterOnSwitch = true;
	}
	RaiseDoor();
	LowerFloorSwitch();
}
void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("Overlap End."));
	if (bCharacterOnSwitch) {
		bCharacterOnSwitch = false;
	}
	// 让动作延时模仿
	GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor,SwitchTime);
}

void AFloorSwitch::UpdateDoorLocation(float z) {
	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += z;
	Door->SetWorldLocation(NewLocation);
}
void AFloorSwitch::UpdateFloorSwitchLocation(float z) {
	FVector NewLocation = InitialSwitchLocation;
	NewLocation.Z += z;
	FloorSwitch->SetWorldLocation(NewLocation);
}

void AFloorSwitch::CloseDoor() {
	if (!bCharacterOnSwitch) { //如果此时没有被点击了，那么就执行关门，如果仍然被点击，则不执行
		LowerDoor();
		RaiseFloorSwitch();
	}
}