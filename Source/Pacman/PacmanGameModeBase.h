//预处理
#pragma once//编译一次
#include "CoreMinimal.h"//核心编程环境
#include "GameFramework/GameModeBase.h"//GameModeBase源代码
#include "PacmanGameModeBase.generated.h"//反射机制
//游戏状态变量
UENUM(BlueprintType)
enum class EGameState : uint8
{
	EMenu,//菜单状态
	EPlay,//游玩状态
	EPause,//暂停状态
	EWin,//胜利状态
	EGameOver//战败状态
};
//游戏模式类
UCLASS()
class PACMAN_API APacmanGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	//构造函数声明
	APacmanGameModeBase();
protected:
	//游戏开始函数声明
	virtual void BeginPlay() override;
public:
	//开始游戏函数声明
	void StartGame();
	//暂停游戏函数声明
	void PauseGame();
	//重启游戏函数声明
	void RestartGame();
	//设置敌人虚弱函数声明
	void SetEnemiesVulnerable();
	//记录敌人定时器函数声明
	void RecordEnemiesTimer();
	//恢复敌人定时器函数声明
	void RestoreEnemiesTimer();
	//获取豆数量函数声明
	int GetPacdotNum() const;
	//设置豆数量函数声明
	void SetPacdotNum(int Value);
	//豆数量变量
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int PacdotNum;
	//设置当前游戏状态函数声明
	EGameState GetCurrentState() const;
	void SetCurrentState(EGameState Value);
	//游戏状态变量
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGameState CurrentState;
private:
	//敌人数组
	TArray<class APacmanEnemy*> Enemies;
	//定时器数组
	TArray<float> EnemiesTimerRemaining;
};
//获取豆数量函数实现
FORCEINLINE int APacmanGameModeBase::GetPacdotNum() const
{
	return PacdotNum;//返回豆数量
}
//获取当前游戏状态函数实现
FORCEINLINE EGameState APacmanGameModeBase::GetCurrentState() const
{
	return CurrentState;//返回当前游戏状态
}