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

	// 카메라 암
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* CameraBoom;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* FollowCamera;


public:

	/*UPROPERTY(Replicated)
	float RunSpeed;*/

	UPROPERTY(ReplicatedUsing = PlayerSpeedUpdateCall)
	bool isRun;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	 
public:	
	/** 프로퍼티 리플리케이션 */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);
	void MoveRight(float value);

	UFUNCTION()
	void PlayerSpeedUpdateCall();

	UFUNCTION()
	void PlayerSpeedUpdate();
protected:
	UFUNCTION()
	void RunStart();
	UFUNCTION()
	void RunStop();

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
};
