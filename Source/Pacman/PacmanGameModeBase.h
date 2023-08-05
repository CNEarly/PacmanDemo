//Ԥ����
#pragma once//����һ��
#include "CoreMinimal.h"//���ı�̻���
#include "GameFramework/GameModeBase.h"//GameModeBaseԴ����
#include "PacmanGameModeBase.generated.h"//�������
//��Ϸ״̬����
UENUM(BlueprintType)
enum class EGameState : uint8
{
	EMenu,//�˵�״̬
	EPlay,//����״̬
	EPause,//��ͣ״̬
	EWin,//ʤ��״̬
	EGameOver//ս��״̬
};
//��Ϸģʽ��
UCLASS()
class PACMAN_API APacmanGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	//���캯������
	APacmanGameModeBase();
protected:
	//��Ϸ��ʼ��������
	virtual void BeginPlay() override;
public:
	//��ʼ��Ϸ��������
	void StartGame();
	//��ͣ��Ϸ��������
	void PauseGame();
	//������Ϸ��������
	void RestartGame();
	//���õ���������������
	void SetEnemiesVulnerable();
	//��¼���˶�ʱ����������
	void RecordEnemiesTimer();
	//�ָ����˶�ʱ����������
	void RestoreEnemiesTimer();
	//��ȡ��������������
	int GetPacdotNum() const;
	//���ö�������������
	void SetPacdotNum(int Value);
	//����������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int PacdotNum;
	//���õ�ǰ��Ϸ״̬��������
	EGameState GetCurrentState() const;
	void SetCurrentState(EGameState Value);
	//��Ϸ״̬����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGameState CurrentState;
private:
	//��������
	TArray<class APacmanEnemy*> Enemies;
	//��ʱ������
	TArray<float> EnemiesTimerRemaining;
};
//��ȡ����������ʵ��
FORCEINLINE int APacmanGameModeBase::GetPacdotNum() const
{
	return PacdotNum;//���ض�����
}
//��ȡ��ǰ��Ϸ״̬����ʵ��
FORCEINLINE EGameState APacmanGameModeBase::GetCurrentState() const
{
	return CurrentState;//���ص�ǰ��Ϸ״̬
}