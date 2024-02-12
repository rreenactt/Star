// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class STAR_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// ī�޶� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* CameraBoom;

	// ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* FollowCamera;


public:

	/*UPROPERTY(Replicated)
	float RunSpeed;*/

	// �������� ������ ���� ���ϸ� �Լ��� ȣ���ϴ� ������
	UPROPERTY(ReplicatedUsing = PlayerSpeedUpdateCall)
	bool isRun;

	UPROPERTY(ReplicatedUsing = PlayerJumpUpdateCall)
	bool isJump;

	UPROPERTY(Replicated)
	bool isAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isPlayerJump;

	bool isAttacking;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
public:	
	/** ������Ƽ ���ø����̼� */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);
	void MoveRight(float value);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void PlayerSpeedUpdateCall();
	UFUNCTION()
	void PlayerJumpUpdateCall();
	UFUNCTION()
	void PlayerAttackUpdateCall();

	UFUNCTION()
	void PlayerSpeedUpdate();
	void PalyerJumpUpdate();
	void PalyerAttackUpdate();

	UFUNCTION()
	virtual void Landed(const FHitResult& Hit);
protected:
	UFUNCTION()
	void RunStart();
	UFUNCTION()
	void RunStop();

	UFUNCTION()
	void JumpStart();
	void JumpEnd();

	UFUNCTION()
	void AttackStart();
	void AttackEnd();

// �޸��� Server, multi
protected:
	UFUNCTION(Reliable, Server = "ServerPlayerRunStart_I", WithValidation = "ServerPlayerRunStart_V")
	virtual void ServerPlayerRunStart(bool run);
	void ServerPlayerRunStart_I(bool run);
	bool ServerPlayerRunStart_V(bool re);

	UFUNCTION(Reliable, Server = "ServerPlayerRunStop_I", WithValidation = "ServerPlayerRunStop_V")
	virtual void ServerPlayerRunStop(bool run);
	void ServerPlayerRunStop_I(bool run);
	bool ServerPlayerRunStop_V(bool re);

	UFUNCTION(Reliable, NetMulticast)
	void MultiPlayerSpeedUpdate(bool run);
	void MultiPlayerSpeedUpdate_Implementation(bool run);

// ���� Server, multi
protected:
	UFUNCTION(Reliable, Server = "ServerPlayerJumpStart_I", WithValidation = "ServerPlayerJumpStart_V")
	virtual void ServerPlayerJumpStart(bool jump);
	void ServerPlayerJumpStart_I(bool jump);
	bool ServerPlayerJumpStart_V(bool jum);

	UFUNCTION(Reliable, Server = "ServerPlayerJumpEnd_I", WithValidation = "ServerPlayerJumpEnd_V")
	virtual void ServerPlayerJumpEnd(bool jump);
	void ServerPlayerJumpEnd_I(bool jump);
	bool ServerPlayerJumpEnd_V(bool jum);

	UFUNCTION(Reliable, NetMulticast)
	virtual void MultiPlayerJumpUpdate(bool jump);
	void MultiPlayerJumpUpdate_Implementation(bool jump);

protected:
	UFUNCTION(Reliable, Server = "ServerPlayerAttackStart_I", WithValidation = "ServerPlayerAttackStart_V")
	virtual void ServerPlayerAttackStart(bool att);
	void ServerPlayerAttackStart_I(bool att);
	bool ServerPlayerAttackStart_V(bool att);

	UFUNCTION(Reliable, Server = "ServerPlayerAttackEnd_I", WithValidation = "ServerPlayerAttackEnd_V")
	virtual void ServerPlayerAttackEnd(bool att);
	void ServerPlayerAttackEnd_I(bool att);
	bool ServerPlayerAttackEnd_V(bool att);

	UFUNCTION(Reliable, NetMulticast)
	virtual void MultiPlayerAttackUpdate();
	void MultiPlayerAttackUpdate_Implementation();

};
