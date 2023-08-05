//Ԥ����
#pragma once//����һ��
#include "CoreMinimal.h"//���ı�̻���
#include "GameFramework/Character.h"//CharacterԴ����
#include "PacmanEnemy.generated.h"//�������
//������
UCLASS()
class PACMAN_API APacmanEnemy : public ACharacter
{
	GENERATED_BODY()
public:
	//���캯������
	APacmanEnemy();
	//�������������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* EnemyDisplayMesh;
	//��������
	bool bIsDead;
	//��������
	bool bIsVulnerable;
	//������ʱ������
	FTimerHandle DeadTimerHandle;
	//������ʱ������
	FTimerHandle VulnerableTimerHandle;
	//����ʱ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeadTime;
	//����ʱ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VulnerableTime;
	//�����ٶȱ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSpeed;
	//�����ٶȱ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VulnerableSpeed;
protected:
	//��Ϸ��ʼ��������
	virtual void BeginPlay() override;
public:	
	//��Ϸÿ֡��������
	virtual void Tick(float DeltaTime) override;
	//������뺯������
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//����������������
	void SetDead(bool bRestoreTimer = false, float Time = 0.0f);
	//����������������
	void SetVulnerable(bool bRestoreTimer = false, float Time = 0.0f);
	//����������������
	void SetNormal();
	//�����ƶ���������
	void SetMovement(bool bCanMove);
	//��ײ��������
	UFUNCTION()
	void OnEnemyBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	//��ʼλ�ñ���
	FVector StartLocation;
};