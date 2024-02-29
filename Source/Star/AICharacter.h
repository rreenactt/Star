// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class STAR_API AAICharacter : public AEntityCharacter
{
	GENERATED_BODY()

public:
	AAICharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool isDie;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	virtual void Die() override;

	void DieProcedure();

	void CanChangeAi();
	UFUNCTION()
	virtual void ChangeAiCharacter();
protected:

	UFUNCTION()
	void AiDiecall();


	UFUNCTION(Reliable, Server = "ServerAiDie_I", WithValidation = "ServerAiDie_V")
	virtual void ServerAiDie();
	void ServerAiDie_I();
	bool ServerAiDie_V();

	UFUNCTION(Reliable, NetMulticast)
	virtual void MultiAiDie();
	void MultiAiDie_Implementation();
};
