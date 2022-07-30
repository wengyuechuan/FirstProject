// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloater : public AActor //Actor不需要声明Blueprintable了，因为已经继承了这个属性了
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloater();
	
	//在UE中可以使用的数学函数
	/*
	* 常用：Abs sin cos rand等 三角函数多用于周期性运动这一类的
	*/

	//这里是创建的静态网格对象
	//网格对象就是一个几何体，可以说是给这个UE对象的皮肤一类的东西
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "ActorMeshComponents")
		UStaticMeshComponent* StaticMesh;    //在游戏编程中如果不是基本变量类型，则更多的使用指针来进行

		/*第一个变量EditInstanceOnly代表该变量只能在Floater的实例下才可以进行操作
		* 当我们基于这个C++类创建一个蓝图类的时候，不能在默认情况下搜索到这个变量，同时也无法更改这个变量
		* 而在我们的关卡编辑界面点击这个实例对象的时候才可以修改
		*/
		//Location used by SetActorLocation() when BeginPlay() is called
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
		FVector initialLocation;


	// Location of the Actor when dragged in from the editor
	//前面的判断VisibleInstanceOnly意味着不希望在实例中对其进行更改，而是只能见到不能进行操作
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
		FVector PlacedLocation;


	/*
	* VisiableDefaultOnly，只能在蓝图界面进行查看，而不能编辑
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Floater Variables")
		FVector WorldOrigin;

	/*
	* 可以在任何地方进行编辑
	* 注意这里的拼写错误，我tm傻了
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	FVector InitialDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	bool bShouldFloat; //用于标志物体是否可以浮动旋转
	/*
	* EditDefaultOnly，可以在蓝图界面进行编辑
	* 可以在默认模式下编辑
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Floater Variables")
	bool bIntializeFloaterLocations;

	/*
	* 在这里设置成为可以修改的变量，这样方便在图形界面进行操作
	*/
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="Floater Variables")
	FVector InitialForce;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector InitialTorque ;

private://private下面不能有EditAnywhere？ 其实是可以有的，这里报错的原因？ 晚上总结的时候记得自己操作以下
	float Runningtime;
	//这里可以看到UPROPERTY的另外一种用法，下面加这个Wave Parameters的下拉效果，我们这里是为了改变正弦波或者是余弦波设置的
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

	//Phase Shift is (C/B)  相位偏移
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
