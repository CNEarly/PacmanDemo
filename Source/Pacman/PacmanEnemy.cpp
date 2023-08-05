//预处理
#include "PacmanEnemy.h"//敌人类
#include "Components/StaticMeshComponent.h"//网格体
#include "Components/CapsuleComponent.h"//碰撞
#include "PacmanPlayer.h"//吃豆人类
#include "GameFramework/CharacterMovementComponent.h"//移动
#include "Engine/Public/TimerManager.h"//定时器
//构造函数实现
APacmanEnemy::APacmanEnemy()
{
	PrimaryActorTick.bCanEverTick = true;//开启tick函数

	bIsDead = false;//阵亡为假
	bIsVulnerable = false;//虚弱为假

	DeadTime = 5.0f;//阵亡时间为5s
	VulnerableTime = 5.0f;//虚弱时间为5s

	NormalSpeed = 300.0f;//正常移动速度为300
	VulnerableSpeed = 100.0f;//虚弱移动速度为100

	EnemyDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyDisplayMesh"));//实例化网格体组件
	EnemyDisplayMesh->SetupAttachment(GetCapsuleComponent());//网格体组件绑定碰撞组件
}
//游戏开始函数实现
void APacmanEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();//设置初始位置为当前位置

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacmanEnemy::OnEnemyBeginOverlay);//胶囊体重叠事件绑定碰撞函数
	SetMovement(true);//开启移动
}
//游戏每帧函数实现
void APacmanEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//玩家输入函数实现
void APacmanEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
//设置阵亡函数实现
void APacmanEnemy::SetDead(bool bRestoreTimer, float Time)
{
	if (bRestoreTimer && bIsDead)//恢复定时器为真且阵亡为真
	{
		GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &APacmanEnemy::SetNormal, Time, false);//阵亡定时器继续
		return;//返回
	}

	if (bIsDead)//阵亡为真
	{
		return;//返回
	}

	bIsDead = true;//阵亡设置为真
	SetMovement(true);//移动设置为真
	SetActorLocation(StartLocation);//位置设置为初始位置

	GetWorldTimerManager().ClearTimer(VulnerableTimerHandle);//清除虚弱定时器
	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &APacmanEnemy::SetNormal, DeadTime, false);//阵亡定时器开启
}
//设置虚弱函数实现
void APacmanEnemy::SetVulnerable(bool bRestoreTimer, float Time)
{
	if (bRestoreTimer)//恢复定时器为真
	{
		GetWorldTimerManager().SetTimer(VulnerableTimerHandle, this, &APacmanEnemy::SetNormal, Time, false);//虚弱定时器继续
	}
	else//恢复定时器未开启
	{
		GetWorldTimerManager().SetTimer(VulnerableTimerHandle, this, &APacmanEnemy::SetNormal, VulnerableTime, false);//虚弱定时器开启
	}

	if (bIsVulnerable)//虚弱为真
	{
		return;//返回
	}

	bIsVulnerable = true;//虚弱设置为真
	SetMovement(true);//移动设置为真
}
//设置正常函数实现
void APacmanEnemy::SetNormal()
{
	GetWorldTimerManager().ClearTimer(DeadTimerHandle);//清除阵亡定时器
	GetWorldTimerManager().ClearTimer(VulnerableTimerHandle);//清除虚弱定时器

	bIsDead = false;//阵亡设置为假
	bIsVulnerable = false;//虚弱设置为假

	SetMovement(true);//设置移动为真
}
//设置移动函数实现
void APacmanEnemy::SetMovement(bool bCanMove)
{
	if (bCanMove)//可移动变量为真
	{
		if (bIsVulnerable == false && bIsDead == false)//虚弱为假且阵亡为假
		{
			GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;//最大速度设置为正常速度
		}
		if (bIsVulnerable == true && bIsDead == false)//虚弱为真且阵亡为假
		{
			GetCharacterMovement()->MaxWalkSpeed = VulnerableSpeed;//最大速度设置为虚弱状态
		}
		if (bIsDead == true)//阵亡为真
		{
			GetCharacterMovement()->MaxWalkSpeed = 0;//最大速度设置为0
		}
	}
	else//可移动变量为假
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;//最大速度设置为0
	}
}
//碰撞函数实现
void APacmanEnemy::OnEnemyBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APacmanPlayer::StaticClass()))//当前碰撞到的是吃豆人
	{
		if (bIsDead)//当敌人是阵亡状态
		{
			return;//忽略
		}

		if (bIsVulnerable)//当敌人是虚弱状态
		{
			SetDead();//阵亡
		}
		else//当敌人是正常状态
		{
			Cast<APacmanPlayer>(OtherActor)->Injured();//吃豆人阵亡
		}
	}
}