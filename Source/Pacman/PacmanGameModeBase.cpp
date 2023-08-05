//Ԥ����
#include "PacmanGameModeBase.h"//��Ϸģʽ��
#include "Pacdot.h"//����
#include "PacmanPlayer.h"//�Զ�����
#include "PacmanEnemy.h"//������
#include "EnemyController.h"//���˿�������
#include "Engine/Public/EngineUtils.h"//��ù���
//���캯��ʵ��
APacmanGameModeBase::APacmanGameModeBase()
{
	CurrentState = EGameState::EMenu;//���ò˵�״̬Ϊ��ǰ��Ϸ״̬
}
//��Ϸ��ʼ����ʵ��
void APacmanGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<APacdot> PacdotItr(GetWorld()); PacdotItr; ++PacdotItr)//�������ж���
	{
		PacdotNum++;//��¼����
	}

	for (TActorIterator<APacmanEnemy> EnemyItr(GetWorld()); EnemyItr; ++EnemyItr)//�������е���
	{
		Enemies.Add(Cast<APacmanEnemy>(*EnemyItr));//�洢����
	}
}
//��ʼ��Ϸ����ʵ��
void APacmanGameModeBase::StartGame()
{
	if (CurrentState == EGameState::EMenu)//��ǰ��Ϸ״̬Ϊ�˵�
	{
		SetCurrentState(EGameState::EPlay);//���õ�ǰ��Ϸ״̬Ϊ����
		for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//�������е��˿�����
		{
			Cast<AEnemyController>((*Iter)->GetController())->GoToNewDestination();//���˽���Ѳ��
		}
	}
}
//��ͣ��Ϸ����ʵ��
void APacmanGameModeBase::PauseGame()
{
	if (GetCurrentState() == EGameState::EPlay)//��ǰ��Ϸ״̬Ϊ����
	{
		RecordEnemiesTimer();//���ü�¼���˶�ʱ������
		SetCurrentState(EGameState::EPause);//���õ�ǰ��Ϸ״̬Ϊ��ͣ
	}
	else if (GetCurrentState() == EGameState::EPause)//��ǰ��Ϸ״̬Ϊ��ͣ
	{
		RestoreEnemiesTimer();//���ûָ����˶�ʱ������
		SetCurrentState(EGameState::EPlay);//���õ�ǰ��Ϸ״̬Ϊ����
	}
}
//������Ϸ����ʵ��
void APacmanGameModeBase::RestartGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("RestartLevel"));//��ȡ��ҿ����������ؿ�
}
//���õ�����������ʵ��
void APacmanGameModeBase::SetEnemiesVulnerable()
{
	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//�������е��˿�����
	{
		(*Iter)->SetVulnerable();//��������Ϊ����
	}
}
//��¼���˶�ʱ������ʵ��
void APacmanGameModeBase::RecordEnemiesTimer()
{
	EnemiesTimerRemaining.Empty();//ʣ�ඨʱ���������

	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//��������
	{
		if ((*Iter)->bIsVulnerable == false && (*Iter)->bIsDead == false)//����δ������δ����
		{
			EnemiesTimerRemaining.Add(-1);
		}
		if ((*Iter)->bIsVulnerable == true && (*Iter)->bIsDead == false)//����������δ����
		{
			float VulnerableTimerRemaining = GetWorldTimerManager().GetTimerRemaining((*Iter)->VulnerableTimerHandle);//����һ����������ʱ����¼ԭ������ʱ��
			EnemiesTimerRemaining.Add(VulnerableTimerRemaining);//��ʱ�����������������ʱ��
		}
		if ((*Iter)->bIsDead == true)//��������
		{
			float DeadTimerRemaining = GetWorldTimerManager().GetTimerRemaining((*Iter)->DeadTimerHandle);//����һ����������ʱ����¼ԭ������ʱ��
			EnemiesTimerRemaining.Add(DeadTimerRemaining);//��ʱ�����������������ʱ��
		}
	}
}
//�ָ����˶�ʱ������ʵ��
void APacmanGameModeBase::RestoreEnemiesTimer()
{
	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//�������е���
	{
		if ((*Iter)->bIsVulnerable == true && (*Iter)->bIsDead == false)//����������δ����
		{
			(*Iter)->SetVulnerable(true, EnemiesTimerRemaining[Iter.GetIndex()]);//���õ�������Ϊ���һָ���ʱ��
		}

		if ((*Iter)->bIsDead == true)//��������
		{
			(*Iter)->SetDead(true, EnemiesTimerRemaining[Iter.GetIndex()]);//���õ�������Ϊ���һָ���ʱ��
		}
	}
}
//���ö���������ʵ��
void APacmanGameModeBase::SetPacdotNum(int Value)
{
	PacdotNum = Value;//���ö���

	if (PacdotNum == 0)//������Ϊ0
	{
		SetCurrentState(EGameState::EWin);//���õ�ǰ״̬Ϊ��ʤ״̬
	}
}
//���õ�ǰ��Ϸ״̬����ʵ��
void APacmanGameModeBase::SetCurrentState(EGameState Value)
{
	CurrentState = Value;//������Ϸ״̬

	switch (CurrentState)//��ȡ������Ϸ״̬Ϊ��
	{
		case EGameState::EPlay://��Ϸ״̬Ϊ����
			for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//�������е���
			{
				(*Iter)->SetMovement(true);//���õ����ƶ�
			}
			Cast<APacmanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(true);//��������ƶ�
			break;
		case EGameState::EWin://��Ϸ״̬Ϊʤ��
			for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//�������е���
			{
				(*Iter)->Destroy();//��������
			}
			break;
		case EGameState::EPause://��Ϸ״̬Ϊ��ͣ
		case EGameState::EGameOver://��Ϸ״̬Ϊս��
			for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)//�������е���
			{
				(*Iter)->SetMovement(false);//���õ��˲��ƶ�
				GetWorldTimerManager().ClearAllTimersForObject(*Iter);//����������ж�ʱ��
			}
			Cast<APacmanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(false);//��������ƶ�
			break;
		default:
			break;
	}
}