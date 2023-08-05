//预处理
#include "Pacdot.h"//豆类
#include "Components/StaticMeshComponent.h"//网格体
#include "Components/SphereComponent.h"//碰撞
//构造函数实现
APacdot::APacdot()
{
	PrimaryActorTick.bCanEverTick = false;//开启tick函数

	PacdotCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PacdotCollision"));//实例化球体碰撞
	PacdotDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacdotDisplayMesh"));//实例化网格体

	PacdotDisplayMesh->SetupAttachment(PacdotCollision);//网格体绑定球体碰撞
}
//游戏开始函数实现
void APacdot::BeginPlay()
{
	Super::BeginPlay();
	
}
//游戏每帧函数实现
void APacdot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}