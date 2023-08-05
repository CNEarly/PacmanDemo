//预处理
#include "EnemyController.h"//敌人控制器类
#include "PacmanEnemy.h"//敌人类
#include "NavigationSystem.h"//导航系统
//占有函数实现
void AEnemyController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	ControlledEnemy = Cast<APacmanEnemy>(PawnToPossess);//控制器占有敌人类
}
//移动完成函数实现
void AEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	GoToNewDestination();//调用移动至新目的地函数
}
//移动至新目的地函数实现
void AEnemyController::GoToNewDestination()
{
	UNavigationSystemV1* NavMesh = UNavigationSystemV1::GetCurrent(this);//获取场景中导航网格的指针
	if (NavMesh)//指针为真
	{
		const float SearchRadius = 10000.0f;//设置巡逻半径
		FNavLocation RandomLocation;//随机位置变量
		const bool bNewDestinationFound = NavMesh->GetRandomReachablePointInRadius(ControlledEnemy->GetActorLocation(), SearchRadius, RandomLocation);//在可到达半径范围内得到一个随机点
		if (bNewDestinationFound)//随机点为真
		{
			MoveToLocation(RandomLocation.Location, 50.0f);//移动至新目的地
		}
	}
}