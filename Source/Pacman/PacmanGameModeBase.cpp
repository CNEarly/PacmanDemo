//预处理
#include "PacmanGameModeBase.h"//游戏模式类
#include "Pacdot.h"//豆类
#include "PacmanPlayer.h"//吃豆人类
#include "PacmanEnemy.h"//敌人类
#include "EnemyController.h"//敌人控制器类
#include "Engine/Public/EngineUtils.h"//虚幻工具
//构造函数实现
APacmanGameModeBase::APacmanGameModeBase()
{
	CurrentState = EGameState::EMenu;//设置菜单状态为当前游戏状态
}
//游戏开始函数实现
void APacmanGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<APacdot> PacdotItr(GetWorld()); PacdotItr; ++PacdotItr)//遍历所有豆子
	{
		PacdotNum++;//记录豆数
	}

	for (TActorIterator<APacmanEnemy> EnemyItr(GetWorld()); EnemyItr; ++EnemyItr)//遍历所有敌人
	{
		Enemies.Add(Cast<APacmanEnemy>(*EnemyItr));//存储敌人
	}
}
//开始游戏函数实现
void APacmanGameModeBase::StartGame()
{
	if (CurrentState == EGameState::EMenu)//当前游戏状态为菜单
	{
		SetCurrentState(EGameState::EPlay);//设置当前游戏状态为游玩
		for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//遍历所有敌人控制器
		{
			Cast<AEnemyController>((*Iter)->GetController())->GoToNewDestination();//敌人进行巡逻
		}
	}
}
//暂停游戏函数实现
void APacmanGameModeBase::PauseGame()
{
	if (GetCurrentState() == EGameState::EPlay)//当前游戏状态为游玩
	{
		RecordEnemiesTimer();//调用记录敌人定时器函数
		SetCurrentState(EGameState::EPause);//设置当前游戏状态为暂停
	}
	else if (GetCurrentState() == EGameState::EPause)//当前游戏状态为暂停
	{
		RestoreEnemiesTimer();//调用恢复敌人定时器函数
		SetCurrentState(EGameState::EPlay);//设置当前游戏状态为游玩
	}
}
//重启游戏函数实现
void APacmanGameModeBase::RestartGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("RestartLevel"));//获取玩家控制器重启关卡
}
//设置敌人虚弱函数实现
void APacmanGameModeBase::SetEnemiesVulnerable()
{
	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//遍历所有敌人控制器
	{
		(*Iter)->SetVulnerable();//敌人设置为虚弱
	}
}
//记录敌人定时器函数实现
void APacmanGameModeBase::RecordEnemiesTimer()
{
	EnemiesTimerRemaining.Empty();//剩余定时器数组清空

	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//遍历敌人
	{
		if ((*Iter)->bIsVulnerable == false && (*Iter)->bIsDead == false)//敌人未虚弱且未阵亡
		{
			EnemiesTimerRemaining.Add(-1);
		}
		if ((*Iter)->bIsVulnerable == true && (*Iter)->bIsDead == false)//敌人虚弱且未阵亡
		{
			float VulnerableTimerRemaining = GetWorldTimerManager().GetTimerRemaining((*Iter)->VulnerableTimerHandle);//定义一个新虚弱定时器记录原虚弱定时器
			EnemiesTimerRemaining.Add(VulnerableTimerRemaining);//定时器数组添加新虚弱定时器
		}
		if ((*Iter)->bIsDead == true)//敌人阵亡
		{
			float DeadTimerRemaining = GetWorldTimerManager().GetTimerRemaining((*Iter)->DeadTimerHandle);//定义一个新阵亡定时器记录原阵亡定时器
			EnemiesTimerRemaining.Add(DeadTimerRemaining);//定时器数组添加新阵亡定时器
		}
	}
}
//恢复敌人定时器函数实现
void APacmanGameModeBase::RestoreEnemiesTimer()
{
	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//遍历所有敌人
	{
		if ((*Iter)->bIsVulnerable == true && (*Iter)->bIsDead == false)//敌人虚弱且未阵亡
		{
			(*Iter)->SetVulnerable(true, EnemiesTimerRemaining[Iter.GetIndex()]);//设置敌人虚弱为真且恢复定时器
		}

		if ((*Iter)->bIsDead == true)//敌人阵亡
		{
			(*Iter)->SetDead(true, EnemiesTimerRemaining[Iter.GetIndex()]);//设置敌人阵亡为真且恢复定时器
		}
	}
}
//设置豆数量函数实现
void APacmanGameModeBase::SetPacdotNum(int Value)
{
	PacdotNum = Value;//设置豆数

	if (PacdotNum == 0)//当豆数为0
	{
		SetCurrentState(EGameState::EWin);//设置当前状态为获胜状态
	}
}
//设置当前游戏状态函数实现
void APacmanGameModeBase::SetCurrentState(EGameState Value)
{
	CurrentState = Value;//设置游戏状态

	switch (CurrentState)//获取到的游戏状态为真
	{
		case EGameState::EPlay://游戏状态为游玩
			for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//遍历所有敌人
			{
				(*Iter)->SetMovement(true);//设置敌人移动
			}
			Cast<APacmanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(true);//设置玩家移动
			break;
		case EGameState::EWin://游戏状态为胜利
			for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//遍历所有敌人
			{
				(*Iter)->Destroy();//敌人销毁
			}
			break;
		case EGameState::EPause://游戏状态为暂停
		case EGameState::EGameOver://游戏状态为战败
			for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//遍历所有敌人
			{
				(*Iter)->SetMovement(false);//设置敌人不移动
				GetWorldTimerManager().ClearAllTimersForObject(*Iter);//清除敌人所有定时器
			}
			Cast<APacmanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(false);//设置玩家移动
			break;
		default:
			break;
	}
}