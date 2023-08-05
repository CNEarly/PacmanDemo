//Ԥ����
#include "PacmanEnemy.h"//������
#include "Components/StaticMeshComponent.h"//������
#include "Components/CapsuleComponent.h"//��ײ
#include "PacmanPlayer.h"//�Զ�����
#include "GameFramework/CharacterMovementComponent.h"//�ƶ�
#include "Engine/Public/TimerManager.h"//��ʱ��
//���캯��ʵ��
APacmanEnemy::APacmanEnemy()
{
	PrimaryActorTick.bCanEverTick = true;//����tick����

	bIsDead = false;//����Ϊ��
	bIsVulnerable = false;//����Ϊ��

	DeadTime = 5.0f;//����ʱ��Ϊ5s
	VulnerableTime = 5.0f;//����ʱ��Ϊ5s

	NormalSpeed = 300.0f;//�����ƶ��ٶ�Ϊ300
	VulnerableSpeed = 100.0f;//�����ƶ��ٶ�Ϊ100

	EnemyDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyDisplayMesh"));//ʵ�������������
	EnemyDisplayMesh->SetupAttachment(GetCapsuleComponent());//�������������ײ���
}
//��Ϸ��ʼ����ʵ��
void APacmanEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();//���ó�ʼλ��Ϊ��ǰλ��

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacmanEnemy::OnEnemyBeginOverlay);//�������ص��¼�����ײ����
	SetMovement(true);//�����ƶ�
}
//��Ϸÿ֡����ʵ��
void APacmanEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//������뺯��ʵ��
void APacmanEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
//������������ʵ��
void APacmanEnemy::SetDead(bool bRestoreTimer, float Time)
{
	if (bRestoreTimer && bIsDead)//�ָ���ʱ��Ϊ��������Ϊ��
	{
		GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &APacmanEnemy::SetNormal, Time, false);//������ʱ������
		return;//����
	}

	if (bIsDead)//����Ϊ��
	{
		return;//����
	}

	bIsDead = true;//��������Ϊ��
	SetMovement(true);//�ƶ�����Ϊ��
	SetActorLocation(StartLocation);//λ������Ϊ��ʼλ��

	GetWorldTimerManager().ClearTimer(VulnerableTimerHandle);//���������ʱ��
	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &APacmanEnemy::SetNormal, DeadTime, false);//������ʱ������
}
//������������ʵ��
void APacmanEnemy::SetVulnerable(bool bRestoreTimer, float Time)
{
	if (bRestoreTimer)//�ָ���ʱ��Ϊ��
	{
		GetWorldTimerManager().SetTimer(VulnerableTimerHandle, this, &APacmanEnemy::SetNormal, Time, false);//������ʱ������
	}
	else//�ָ���ʱ��δ����
	{
		GetWorldTimerManager().SetTimer(VulnerableTimerHandle, this, &APacmanEnemy::SetNormal, VulnerableTime, false);//������ʱ������
	}

	if (bIsVulnerable)//����Ϊ��
	{
		return;//����
	}

	bIsVulnerable = true;//��������Ϊ��
	SetMovement(true);//�ƶ�����Ϊ��
}
//������������ʵ��
void APacmanEnemy::SetNormal()
{
	GetWorldTimerManager().ClearTimer(DeadTimerHandle);//���������ʱ��
	GetWorldTimerManager().ClearTimer(VulnerableTimerHandle);//���������ʱ��

	bIsDead = false;//��������Ϊ��
	bIsVulnerable = false;//��������Ϊ��

	SetMovement(true);//�����ƶ�Ϊ��
}
//�����ƶ�����ʵ��
void APacmanEnemy::SetMovement(bool bCanMove)
{
	if (bCanMove)//���ƶ�����Ϊ��
	{
		if (bIsVulnerable == false && bIsDead == false)//����Ϊ��������Ϊ��
		{
			GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;//����ٶ�����Ϊ�����ٶ�
		}
		if (bIsVulnerable == true && bIsDead == false)//����Ϊ��������Ϊ��
		{
			GetCharacterMovement()->MaxWalkSpeed = VulnerableSpeed;//����ٶ�����Ϊ����״̬
		}
		if (bIsDead == true)//����Ϊ��
		{
			GetCharacterMovement()->MaxWalkSpeed = 0;//����ٶ�����Ϊ0
		}
	}
	else//���ƶ�����Ϊ��
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;//����ٶ�����Ϊ0
	}
}
//��ײ����ʵ��
void APacmanEnemy::OnEnemyBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APacmanPlayer::StaticClass()))//��ǰ��ײ�����ǳԶ���
	{
		if (bIsDead)//������������״̬
		{
			return;//����
		}

		if (bIsVulnerable)//������������״̬
		{
			SetDead();//����
		}
		else//������������״̬
		{
			Cast<APacmanPlayer>(OtherActor)->Injured();//�Զ�������
		}
	}
}