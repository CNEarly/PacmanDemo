//Ԥ����
#include "EnemyController.h"//���˿�������
#include "PacmanEnemy.h"//������
#include "NavigationSystem.h"//����ϵͳ
//ռ�к���ʵ��
void AEnemyController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	ControlledEnemy = Cast<APacmanEnemy>(PawnToPossess);//������ռ�е�����
}
//�ƶ���ɺ���ʵ��
void AEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	GoToNewDestination();//�����ƶ�����Ŀ�ĵغ���
}
//�ƶ�����Ŀ�ĵغ���ʵ��
void AEnemyController::GoToNewDestination()
{
	UNavigationSystemV1* NavMesh = UNavigationSystemV1::GetCurrent(this);//��ȡ�����е��������ָ��
	if (NavMesh)//ָ��Ϊ��
	{
		const float SearchRadius = 10000.0f;//����Ѳ�߰뾶
		FNavLocation RandomLocation;//���λ�ñ���
		const bool bNewDestinationFound = NavMesh->GetRandomReachablePointInRadius(ControlledEnemy->GetActorLocation(), SearchRadius, RandomLocation);//�ڿɵ���뾶��Χ�ڵõ�һ�������
		if (bNewDestinationFound)//�����Ϊ��
		{
			MoveToLocation(RandomLocation.Location, 50.0f);//�ƶ�����Ŀ�ĵ�
		}
	}
}