// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "Net/UnrealNetwork.h"

AAICharacter::AAICharacter()
{
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	/*FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		Die();

		GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
	}), 15.0f, false);*/
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAICharacter::Die()
{
	GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Red, TEXT("Die"));
	GetMesh()->SetSimulatePhysics(true);
	ServerAiDie();
}

void AAICharacter::AiDiecall()
{
	MultiAiDie();
}

void AAICharacter::ServerAiDie_I()
{
	GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Red, TEXT("Die"));
	GetMesh()->SetSimulatePhysics(true);
	AiDiecall();
}

bool AAICharacter::ServerAiDie_V()
{
	return true;
}

void AAICharacter::MultiAiDie_Implementation()
{
	GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Red, TEXT("Die"));
	GetMesh()->SetSimulatePhysics(true);
}
