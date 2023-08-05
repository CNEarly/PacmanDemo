//Ԥ����
#pragma once//����һ��
#include "CoreMinimal.h"//���ı�̻���
#include "GameFramework/Character.h"//CharacterԴ����
#include "PacmanPlayer.generated.h"//�������
//�Զ���
UCLASS()
class PACMAN_API APacmanPlayer : public ACharacter
{
	GENERATED_BODY()
public:
	//���캯������
	APacmanPlayer();
	//�������������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PacmanDisplayMesh;
	//����������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Lifes;
	//�޵�ʱ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InvincibleTime;
	//�����ٶȱ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSpeed;
protected:
	//��Ϸ��ʼ��������
	virtual void BeginPlay() override;
public:	
	//��Ϸÿ֡��������
	virtual void Tick(float DeltaTime) override;
	//������뺯������
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//���˺�������
	void Injured();
	//��λ��������
	void Resetting();
	//�����ƶ���������
	void SetMovement(bool bCanMove);
	//��ʼ��Ϸ��������
	void StartGame();
	//��ͣ��Ϸ��������
	void PauseGame();
	//������Ϸ��������
	void RestartGame();
	//�����ƶ���������
	void MoveX(float Value);
	//�����ƶ���������
	void MoveY(float Value);
	//��ײ��������
	UFUNCTION()
	void OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	//��Ϸ��ʼλ�ñ���
	FVector StartLocation;
	//�ƶ���������
	FVector Velocity;
	//��Ϸģʽ��
	class APacmanGameModeBase* GameModeRef;
	//��λ��ʱ��
	FTimerHandle ResettingTimerHandle;
};