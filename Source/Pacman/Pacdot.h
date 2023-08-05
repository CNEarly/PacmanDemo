//预处理
#pragma once//编译一次
#include "CoreMinimal.h"//核心编程环境
#include "GameFramework/Actor.h"//Actor源代码
#include "Pacdot.generated.h"//反射机制
//豆类
UCLASS(Blueprintable)
class PACMAN_API APacdot : public AActor
{
	GENERATED_BODY()
public:	
	//构造函数声明
	APacdot();
	//球体碰撞组件变量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* PacdotCollision;
	//网格体组件变量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PacdotDisplayMesh;
	//超级豆属性变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSuperPacdot = false;
protected:
	//游戏开始函数声明
	virtual void BeginPlay() override;
public:	
	//游戏每帧函数声明
	virtual void Tick(float DeltaTime) override;
};