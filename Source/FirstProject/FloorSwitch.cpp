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
	//TriggerBox�����ƶ�����������������������壬��Ҫ�����ƶ��������Ļ������Ǹ���������ó�TriggerBox�Ϳ�����
	//��������������ܲ�̫ȫ�棬����������ʵ�������������������������Ķ��������������������ǿ��Եõ�������Ҫ�Ľ��
	RootComponent = TriggerBox;
	
	/*
	* ������ײ����
	* QueryOnly��ֻ���ص���� ���滹�кܶ಻ͬ��ֵ���������ܽ��ʱ����Ū
	*/
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	/*
	* ����ĵ�������ôһ����Ѷ���Ȼ���Ҳ�ѯ��һƪ�ȽϺõ����£��������������
	* https://zhuanlan.zhihu.com/p/91669703
	* https://zhuanlan.zhihu.com/p/70045930?from_voters_page=true Gamemode���ƣ���Щ���¿��Զ��һ����Ȼ������һ��GamePlay�Ļ���
	* ���������ײͨ�����������Ϊ������ײ������ı������ͣ���Ϊ
	* WorldStatic-��Ϸ�еľ�̬���󣬼��������κη�ʽ�ƶ�������εȡ�
	* WorldDynamic-��������4��֮�����Ϸ�ڿ��ƶ����塣
	* Pawn-����һ���AI���ƵĽ�ɫ��
	* PhysicsBody-���п��Ա�Simulate��Physics Body����Object��
	* Vehicle-���ΪVehicle����ҵ�Pawn���Խ������С�
	* Destructible-�κο��ƻ���Actor����ʹ��Destructible Mesh Editor���б༭��
	* ������������ƵĿ�������趨���ǲ������κη�ʽ�ƶ�����������ѡ����WorldStatic�������
	* ֮������ݿ�����ȫ�ο����£�
	* д�ķǳ�������������ҵĵ�һ����Ϸ��Ŀ֮���ҽ���ʼ�Ķ�����Ĺ�������ײ�Ķ���
	*/
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	//�����������ײ����Ӧ���� �Ⱥ������е����ͣ�Ȼ�������ض������ͷ�Ӧ Ҳ���Ƕ���ҿɲ��ݽ�ɫ���з�Ӧ
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); 
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//������״
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
	//Overlap���ص����������������뵱ǰ�����ص���������ײ�ȣ�
	//AddDynamic�Ļ��ƺ�C#��ί�У�delegate�����Ч�������Խ��¼����а󶨣�����Ҳ�ǽ��¼����а󶨵ĺ���
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
/// ��֮�󣬵���ɫ������غϣ��ͻ�����������������һЩ��Ϣ�����������ݵ������������
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
	// �ö�����ʱģ��
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
	if (!bCharacterOnSwitch) { //�����ʱû�б�����ˣ���ô��ִ�й��ţ������Ȼ���������ִ��
		LowerDoor();
		RaiseFloorSwitch();
	}
}