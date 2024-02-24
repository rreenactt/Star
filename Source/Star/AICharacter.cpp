// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "MyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

AAICharacter::AAICharacter()
{
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FTimerHandle myTimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
	//{
	//	ChangeAiCharacter();

	//	GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
	//}), 5.0f, false);
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

void AAICharacter::DieProcedure()
{
	// 캡슐콜리전 삭제
	UCapsuleComponent* AiCapsuleComponent = Cast<UCapsuleComponent>(GetCapsuleComponent());
	// 컨트롤러 삭제
	AAIController* Aicontroller = Cast<AAIController>(GetController());
	if (Aicontroller && AiCapsuleComponent)
	{
		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Red, TEXT("Die"));

		Aicontroller->Destroyed();
		AiCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	GetMesh()->SetSimulatePhysics(true);
}

void AAICharacter::AiDiecall()
{
	MultiAiDie();
}

void AAICharacter::ChangeAiCharacter()
{
	int32 RandomNumber = FMath::RandRange(1, 3);

	ChangeCharacter(RandomNumber);
}

void AAICharacter::ServerAiDie_I()
{
	// 캡슐콜리전 삭제
	UCapsuleComponent* AiCapsuleComponent = Cast<UCapsuleComponent>(GetCapsuleComponent());
	// 컨트롤러 삭제
	AAIController* Aicontroller = Cast<AAIController>(GetController());
	if (Aicontroller && AiCapsuleComponent)
	{
		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Red, TEXT("Die"));

		Aicontroller->Destroyed();
		AiCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	GetMesh()->SetSimulatePhysics(true);
	AiDiecall();
}

bool AAICharacter::ServerAiDie_V()
{
	return true;
}

void AAICharacter::MultiAiDie_Implementation()
{
	DieProcedure();
}
