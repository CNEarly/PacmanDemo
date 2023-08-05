//预处理
#pragma once//编译一次
#include "CoreMinimal.h"//核心编程环境
#include "GameFramework/Character.h"//Character源代码
#include "PacmanPlayer.generated.h"//反射机制
//吃豆类
UCLASS()
class PACMAN_API APacmanPlayer : public ACharacter
{
	GENERATED_BODY()
public:
	//构造函数声明
	APacmanPlayer();
	//网格体组件声明
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PacmanDisplayMesh;
	//生命数变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Lifes;
	//无敌时间变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InvincibleTime;
	//正常速度变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSpeed;
protected:
	//游戏开始函数声明
	virtual void BeginPlay() override;
public:	
	//游戏每帧函数声明
	virtual void Tick(float DeltaTime) override;
	//玩家输入函数声明
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//受伤函数声明
	void Injured();
	//复位函数声明
	void Resetting();
	//设置移动函数声明
	void SetMovement(bool bCanMove);
	//开始游戏函数声明
	void StartGame();
	//暂停游戏函数声明
	void PauseGame();
	//重启游戏函数声明
	void RestartGame();
	//左右移动函数声明
	void MoveX(float Value);
	//上下移动函数声明
	void MoveY(float Value);
	//碰撞函数声明
	UFUNCTION()
	void OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	//游戏开始位置变量
	FVector StartLocation;
	//移动向量变量
	FVector Velocity;
	//游戏模式类
	class APacmanGameModeBase* GameModeRef;
	//复位定时器
	FTimerHandle ResettingTimerHandle;
};