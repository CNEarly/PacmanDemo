//预处理
#pragma once//编译一次
#include "CoreMinimal.h"//核心编程环境
#include "AIController.h"//AIController源代码
#include "EnemyController.generated.h"//反射机制
//敌人控制器类
UCLASS()
class PACMAN_API AEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	//占有函数声明
	void OnPossess(class APawn* PawnToPossess) override;
	//移动完成函数声明
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	//移动至新目的地函数声明
	void GoToNewDestination();
private:
	//控制敌人变量
	class APacmanEnemy* ControlledEnemy;
};