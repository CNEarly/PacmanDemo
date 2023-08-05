//预处理
#pragma once//编译一次
#include "CoreMinimal.h"//核心编程环境
#include "GameFramework/Character.h"//Character源代码
#include "PacmanEnemy.generated.h"//反射机制
//敌人类
UCLASS()
class PACMAN_API APacmanEnemy : public ACharacter
{
	GENERATED_BODY()
public:
	//构造函数声明
	APacmanEnemy();
	//网格体组件声明
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* EnemyDisplayMesh;
	//阵亡变量
	bool bIsDead;
	//虚弱变量
	bool bIsVulnerable;
	//阵亡定时器变量
	FTimerHandle DeadTimerHandle;
	//虚弱定时器变量
	FTimerHandle VulnerableTimerHandle;
	//阵亡时间变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeadTime;
	//虚弱时间变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VulnerableTime;
	//正常速度变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSpeed;
	//虚弱速度变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VulnerableSpeed;
protected:
	//游戏开始函数声明
	virtual void BeginPlay() override;
public:	
	//游戏每帧函数声明
	virtual void Tick(float DeltaTime) override;
	//玩家输入函数声明
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//设置阵亡函数声明
	void SetDead(bool bRestoreTimer = false, float Time = 0.0f);
	//设置虚弱函数声明
	void SetVulnerable(bool bRestoreTimer = false, float Time = 0.0f);
	//设置正常函数声明
	void SetNormal();
	//设置移动函数声明
	void SetMovement(bool bCanMove);
	//碰撞函数声明
	UFUNCTION()
	void OnEnemyBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	//起始位置变量
	FVector StartLocation;
};