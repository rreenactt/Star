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

	//UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
	float RunSpeed;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	 
public:	
	/** ������Ƽ ���ø����̼� */
	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);
	void MoveRight(float value);

protected:
	UFUNCTION()
	void RunStart();
	UFUNCTION()
	void RunStop();
	UFUNCTION()
	void OnRep_CurrentWalkSpeed();
};
