//预处理
#include "PacmanPlayer.h"//吃豆人类
#include "Components/StaticMeshComponent.h"//网格体
#include "Components/CapsuleComponent.h"//碰撞
#include "Components/InputComponent.h"//输入
#include "Pacdot.h"//豆类
#include "Kismet/GameplayStatics.h"//游戏状态
#include "PacmanGameModeBase.h"//游戏模式类
#include "Engine/Public/TimerManager.h"//定时器
#include "GameFramework/CharacterMovementComponent.h"//移动
//构造函数实现
APacmanPlayer::APacmanPlayer()
{
	PrimaryActorTick.bCanEverTick = true;//开启tick函数

	Lifes = 3;//生命数为3
	InvincibleTime = 3.0f;//无敌时间为3s
	NormalSpeed = 600.0f;//正常速度为600
	Velocity = FVector(0, 0, 0);//移动向量为（0，0，0）

	PacmanDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanDisplayMesh"));//实例化网格体组件
	PacmanDisplayMesh->SetupAttachment(GetCapsuleComponent());//网格体组件绑定胶囊体组件
}
//游戏开始函数实现
void APacmanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();//设置当前位置为起始位置
	GameModeRef = Cast<APacmanGameModeBase>(UGameplayStatics::GetGameMode(this));//类型检测获取当前游戏模式

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacmanPlayer::OnPacmanBeginOverlay);//胶囊体重叠事件绑定碰撞函数
	SetMovement(true);//开启移动
}
//游戏每帧函数实现
void APacmanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//玩家输入函数实现
void APacmanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("StartGame", IE_Pressed, this, &APacmanPlayer::StartGame);//开始游戏操作映射
	PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &APacmanPlayer::PauseGame);//暂停游戏操作映射
	PlayerInputComponent->BindAction("RestartGame", IE_Pressed, this, &APacmanPlayer::RestartGame);//重启游戏操作映射

	PlayerInputComponent->BindAxis("MoveX", this, &APacmanPlayer::MoveX);//左右移动轴映射
	PlayerInputComponent->BindAxis("MoveY", this, &APacmanPlayer::MoveY);//上下移动轴映射
}
//受伤函数实现
void APacmanPlayer::Injured()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Invincible"));

	Lifes--;//生命数减一
	if (Lifes == 0)//生命数为0时
	{
		GameModeRef->SetCurrentState(EGameState::EGameOver);//游戏模式设置为游戏结束
	}
	else//生命数不为0时
	{
		SetActorLocation(StartLocation);//吃豆人位置设置为初始位置
		GetWorldTimerManager().SetTimer(ResettingTimerHandle, this, &APacmanPlayer::Resetting, InvincibleTime, false);//启动复位定时器
	}
}
//复位函数实现
void APacmanPlayer::Resetting()
{
	GetWorldTimerManager().ClearTimer(ResettingTimerHandle);//清除复位定时器
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));//设置碰撞预设为pawn
}
//设置移动函数实现
void APacmanPlayer::SetMovement(bool bCanMove)
{
	if (bCanMove)//玩家可以移动时最大速度为正常速度
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
	else//玩家不可以移动时最大速度为0
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}
//开始游戏函数实现
void APacmanPlayer::StartGame()
{
	GameModeRef->StartGame();//调用游戏模式类开始游戏函数
}
//暂停游戏函数实现
void APacmanPlayer::PauseGame()
{
	GameModeRef->PauseGame();//调用游戏模式类暂停游戏函数
}
//重启游戏函数实现
void APacmanPlayer::RestartGame()
{
	GameModeRef->RestartGame();//调用游戏模式类重启游戏函数
}
//左右移动函数实现
void APacmanPlayer::MoveX(float Value)
{
	if (GameModeRef->GetCurrentState() == EGameState::EPlay)//游戏模式为游戏游玩时
	{
		Velocity.X = Value;//移动向量X轴变量为传参
		Velocity.Y = 0;//移动向量Y轴变量为0

		if (Value > 0)//X轴变量大于0
		{
			SetActorRotation(FRotator(0, 0, 0));//设置旋转为0度朝向
		}
		else if (Value < 0)//X轴变量小于0
		{
			SetActorRotation(FRotator(0, -180, 0)); //设置旋转为-180度朝向
		}

		AddMovementInput(Velocity);//将移动向量传入移动输入函数
	}
}
//上下移动函数实现
void APacmanPlayer::MoveY(float Value)
{
	if (GameModeRef->GetCurrentState() == EGameState::EPlay)//游戏模式为游戏游玩时
	{
		Velocity.X = 0;//移动向量X轴变量为0
		Velocity.Y = Value;//移动向量Y轴变量为传参

		if (Value > 0)//Y轴变量大于0
		{
			SetActorRotation(FRotator(0, 90, 0));//设置旋转为90度朝向
		}
		else if (Value < 0)//Y轴变量小于0
		{
			SetActorRotation(FRotator(0, -90, 0));//设置旋转为-90度朝向
		}

		AddMovementInput(Velocity);//将移动向量传入移动输入函数
	}
}
//碰撞函数实现
void APacmanPlayer::OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GameModeRef->GetCurrentState() == EGameState::EPlay)//游戏模式为游戏游玩时
	{
		if (OtherActor->IsA(APacdot::StaticClass()))//当前碰撞到的是豆
		{
			APacdot* CurrentPacdot = Cast<APacdot>(OtherActor);//保存当前碰撞的豆
			if (CurrentPacdot->bIsSuperPacdot)//碰撞超级豆
			{
				GameModeRef->SetEnemiesVulnerable();//游戏模式调用敌人设置虚弱状态函数
			}
			OtherActor->Destroy();//销毁碰撞的豆
			GameModeRef->SetPacdotNum(GameModeRef->GetPacdotNum() - 1);//游戏模式记录豆数-1
		}
	}
}