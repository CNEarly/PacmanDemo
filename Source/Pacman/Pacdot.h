//Ԥ����
#pragma once//����һ��
#include "CoreMinimal.h"//���ı�̻���
#include "GameFramework/Actor.h"//ActorԴ����
#include "Pacdot.generated.h"//�������
//����
UCLASS(Blueprintable)
class PACMAN_API APacdot : public AActor
{
	GENERATED_BODY()
public:	
	//���캯������
	APacdot();
	//������ײ�������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* PacdotCollision;
	//�������������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PacdotDisplayMesh;
	//���������Ա���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSuperPacdot = false;
protected:
	//��Ϸ��ʼ��������
	virtual void BeginPlay() override;
public:	
	//��Ϸÿ֡��������
	virtual void Tick(float DeltaTime) override;
};