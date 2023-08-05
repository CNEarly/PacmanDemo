//Ԥ����
#include "PacmanPlayer.h"//�Զ�����
#include "Components/StaticMeshComponent.h"//������
#include "Components/CapsuleComponent.h"//��ײ
#include "Components/InputComponent.h"//����
#include "Pacdot.h"//����
#include "Kismet/GameplayStatics.h"//��Ϸ״̬
#include "PacmanGameModeBase.h"//��Ϸģʽ��
#include "Engine/Public/TimerManager.h"//��ʱ��
#include "GameFramework/CharacterMovementComponent.h"//�ƶ�
//���캯��ʵ��
APacmanPlayer::APacmanPlayer()
{
	PrimaryActorTick.bCanEverTick = true;//����tick����

	Lifes = 3;//������Ϊ3
	InvincibleTime = 3.0f;//�޵�ʱ��Ϊ3s
	NormalSpeed = 600.0f;//�����ٶ�Ϊ600
	Velocity = FVector(0, 0, 0);//�ƶ�����Ϊ��0��0��0��

	PacmanDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanDisplayMesh"));//ʵ�������������
	PacmanDisplayMesh->SetupAttachment(GetCapsuleComponent());//����������󶨽��������
}
//��Ϸ��ʼ����ʵ��
void APacmanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();//���õ�ǰλ��Ϊ��ʼλ��
	GameModeRef = Cast<APacmanGameModeBase>(UGameplayStatics::GetGameMode(this));//���ͼ���ȡ��ǰ��Ϸģʽ

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacmanPlayer::OnPacmanBeginOverlay);//�������ص��¼�����ײ����
	SetMovement(true);//�����ƶ�
}
//��Ϸÿ֡����ʵ��
void APacmanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//������뺯��ʵ��
void APacmanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("StartGame", IE_Pressed, this, &APacmanPlayer::StartGame);//��ʼ��Ϸ����ӳ��
	PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &APacmanPlayer::PauseGame);//��ͣ��Ϸ����ӳ��
	PlayerInputComponent->BindAction("RestartGame", IE_Pressed, this, &APacmanPlayer::RestartGame);//������Ϸ����ӳ��

	PlayerInputComponent->BindAxis("MoveX", this, &APacmanPlayer::MoveX);//�����ƶ���ӳ��
	PlayerInputComponent->BindAxis("MoveY", this, &APacmanPlayer::MoveY);//�����ƶ���ӳ��
}
//���˺���ʵ��
void APacmanPlayer::Injured()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Invincible"));

	Lifes--;//��������һ
	if (Lifes == 0)//������Ϊ0ʱ
	{
		GameModeRef->SetCurrentState(EGameState::EGameOver);//��Ϸģʽ����Ϊ��Ϸ����
	}
	else//��������Ϊ0ʱ
	{
		SetActorLocation(StartLocation);//�Զ���λ������Ϊ��ʼλ��
		GetWorldTimerManager().SetTimer(ResettingTimerHandle, this, &APacmanPlayer::Resetting, InvincibleTime, false);//������λ��ʱ��
	}
}
//��λ����ʵ��
void APacmanPlayer::Resetting()
{
	GetWorldTimerManager().ClearTimer(ResettingTimerHandle);//�����λ��ʱ��
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));//������ײԤ��Ϊpawn
}
//�����ƶ�����ʵ��
void APacmanPlayer::SetMovement(bool bCanMove)
{
	if (bCanMove)//��ҿ����ƶ�ʱ����ٶ�Ϊ�����ٶ�
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
	else//��Ҳ������ƶ�ʱ����ٶ�Ϊ0
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}
//��ʼ��Ϸ����ʵ��
void APacmanPlayer::StartGame()
{
	GameModeRef->StartGame();//������Ϸģʽ�࿪ʼ��Ϸ����
}
//��ͣ��Ϸ����ʵ��
void APacmanPlayer::PauseGame()
{
	GameModeRef->PauseGame();//������Ϸģʽ����ͣ��Ϸ����
}
//������Ϸ����ʵ��
void APacmanPlayer::RestartGame()
{
	GameModeRef->RestartGame();//������Ϸģʽ��������Ϸ����
}
//�����ƶ�����ʵ��
void APacmanPlayer::MoveX(float Value)
{
	if (GameModeRef->GetCurrentState() == EGameState::EPlay)//��ϷģʽΪ��Ϸ����ʱ
	{
		Velocity.X = Value;//�ƶ�����X�����Ϊ����
		Velocity.Y = 0;//�ƶ�����Y�����Ϊ0

		if (Value > 0)//X���������0
		{
			SetActorRotation(FRotator(0, 0, 0));//������תΪ0�ȳ���
		}
		else if (Value < 0)//X�����С��0
		{
			SetActorRotation(FRotator(0, -180, 0)); //������תΪ-180�ȳ���
		}

		AddMovementInput(Velocity);//���ƶ����������ƶ����뺯��
	}
}
//�����ƶ�����ʵ��
void APacmanPlayer::MoveY(float Value)
{
	if (GameModeRef->GetCurrentState() == EGameState::EPlay)//��ϷģʽΪ��Ϸ����ʱ
	{
		Velocity.X = 0;//�ƶ�����X�����Ϊ0
		Velocity.Y = Value;//�ƶ�����Y�����Ϊ����

		if (Value > 0)//Y���������0
		{
			SetActorRotation(FRotator(0, 90, 0));//������תΪ90�ȳ���
		}
		else if (Value < 0)//Y�����С��0
		{
			SetActorRotation(FRotator(0, -90, 0));//������תΪ-90�ȳ���
		}

		AddMovementInput(Velocity);//���ƶ����������ƶ����뺯��
	}
}
//��ײ����ʵ��
void APacmanPlayer::OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GameModeRef->GetCurrentState() == EGameState::EPlay)//��ϷģʽΪ��Ϸ����ʱ
	{
		if (OtherActor->IsA(APacdot::StaticClass()))//��ǰ��ײ�����Ƕ�
		{
			APacdot* CurrentPacdot = Cast<APacdot>(OtherActor);//���浱ǰ��ײ�Ķ�
			if (CurrentPacdot->bIsSuperPacdot)//��ײ������
			{
				GameModeRef->SetEnemiesVulnerable();//��Ϸģʽ���õ�����������״̬����
			}
			OtherActor->Destroy();//������ײ�Ķ�
			GameModeRef->SetPacdotNum(GameModeRef->GetPacdotNum() - 1);//��Ϸģʽ��¼����-1
		}
	}
}