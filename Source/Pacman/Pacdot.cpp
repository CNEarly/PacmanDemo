//Ԥ����
#include "Pacdot.h"//����
#include "Components/StaticMeshComponent.h"//������
#include "Components/SphereComponent.h"//��ײ
//���캯��ʵ��
APacdot::APacdot()
{
	PrimaryActorTick.bCanEverTick = false;//����tick����

	PacdotCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PacdotCollision"));//ʵ����������ײ
	PacdotDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacdotDisplayMesh"));//ʵ����������

	PacdotDisplayMesh->SetupAttachment(PacdotCollision);//�������������ײ
}
//��Ϸ��ʼ����ʵ��
void APacdot::BeginPlay()
{
	Super::BeginPlay();
	
}
//��Ϸÿ֡����ʵ��
void APacdot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}