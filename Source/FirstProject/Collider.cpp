// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include"Components/SphereComponent.h" //�����������˽��������ķ���
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

	//SphereComponent�����������ڴ�����ײ��һ�ද������ô����������Ϊ�µĸ� ��������þͿ���ɾ������
	//���ú��������ñ������ø�������Ǻܺõķ�ʽ��UE�����ṩ�˶��ַ���
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->InitSphereRadius(40.0f);//������ײ�����ֱ��
	//������ײ��ĵ������ƣ����������Ϊ����ײͨ����ȡʱ������
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	/// <summary>
	/// ������һ�����õ���⣬����˵���������ǵ�ǰ������Ӿ�չʾ����Ȼ�����Ծ���һЩ��������
	/// </summary>
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	//�ڴ����п��Զ����Ǵ�����һЩ���Խ����޸ģ�����һ�����ڱ༭����ͼ�ν��������޸�
	//��������ķ������Խ���һ�����޸ģ��������洴��һ��ConstructorHelpers����Ȼ��ͨ����������ҵ����ǵľ�̬������Դ��Ȼ�����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshComponentAsset.Succeeded()) { //������Դ�ɹ�
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);//����ǰ�ľ�̬�������ó�Ϊ����ѡ����������
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f)); //�ı�λ�ã�λ�ã�
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f)); //������Ա��������ţ�
	}
	//SpringArm�ǵ��ɱۣ���������֪�������������ʲô�����ɱ���������ÿ��Ը���Gameplay�����������չ�ͻ�����Ҳ�����൱��һ��֧�������������Ȼ����������Ŵ���С�����Ĺ���
	//������ֿ��Զ��ĵ� https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/UsingCameras/SpringArmComponents/
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	//�ڶ��������ǰ������������
	Camera->SetupAttachment(ACollider::GetSpringArmComponent(),USpringArmComponent::SocketName);

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("OurMovementComponent"));
	OurMovementComponent->UpdatedComponent = GetRootComponent();//���������������ȷ�����MovementComponent���Ƶ���˭��Ҳ����˵

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

	NewRotation.Yaw += CameraInput.X;//��ת����� ���ı�Yaw
	SetActorRotation(NewRotation);

	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += CameraInput.Y, -80.f, -15.f);

	SpringArm->SetWorldRotation(NewSpringArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//���̰����İ�
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this,&ACollider::MoveRight);

	//�����ƶ�
	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::YawCamera);
}
/*
* ������Ȼ�޷��˶���ԭ��
*	Pawn�����Լ�����MovementComponent������Ҫ�����Զ������������ע��Character���Զ�������������������ϰʹ��������룬�������
*	UPawnMovementComponent* MovementComponent = GetMovementComponent();
*	������������AddMovementInput�ĵ�һ�У�Ҳ���ǻ�õ�ǰ��MovementComponent���������ǵ�ǰ��û�а�����ֻ���ǽ��յ�һ��False��Ҳ���ǿգ�
*	������Ҳ��û�취ִ���ˣ���Ҳ����Ϊʲô�����Լ����������岻�ܸ�����̰��������˶�
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

//���������أ���д�Լ��ض��嶼��Ҫ���¸�ϰһ�£���������ݻ��Ǻ���
UPawnMovementComponent* ACollider::GetMovementComponent() const{
	return OurMovementComponent;
}

void ACollider::YawCamera(float AxisValue) {
	CameraInput.X = AxisValue;
}
void ACollider::PitchCamera(float AxisValue) {
	CameraInput.Y = AxisValue;
}
