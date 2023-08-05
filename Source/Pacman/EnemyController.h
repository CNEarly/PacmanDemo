//Ԥ����
#pragma once//����һ��
#include "CoreMinimal.h"//���ı�̻���
#include "AIController.h"//AIControllerԴ����
#include "EnemyController.generated.h"//�������
//���˿�������
UCLASS()
class PACMAN_API AEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	//ռ�к�������
	void OnPossess(class APawn* PawnToPossess) override;
	//�ƶ���ɺ�������
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	//�ƶ�����Ŀ�ĵغ�������
	void GoToNewDestination();
private:
	//���Ƶ��˱���
	class APacmanEnemy* ControlledEnemy;
};