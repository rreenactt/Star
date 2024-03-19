// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AIController.h"
#include "MyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

AAICharacter::AAICharacter()
{
	isDie = false;
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	CanChangeAi();
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
	UCapsuleComponent* AiCapsuleComponent = Cast<UCapsuleComponent>(GetCapsuleComponent());

	if (AiCapsuleComponent && AiCapsuleComponent->GetCollisionEnabled() != (ECollisionEnabled::NoCollision))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Die and all destroyed!"));
		isDie = true;
		GetMesh()->SetSimulatePhysics(true);
		AiCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//ServerAiDie();

	}
	
}

void AAICharacter::DieProcedure()
{
	UCapsuleComponent* AiCapsuleComponent = Cast<UCapsuleComponent>(GetCapsuleComponent());
	AMyAIController* Aicontroller = Cast<AMyAIController>(GetController());
	if (AiCapsuleComponent && AiCapsuleComponent->GetCollisionEnabled() != (ECollisionEnabled::NoCollision))
	{
		isDie = true;
		GetMesh()->SetSimulatePhysics(true);
		AiCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}


void AAICharacter::CanChangeAi()
{
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if (isDie)
		{
			return;
		}
		int32 RandomNum = FMath::RandRange(1, 7);
		if (RandomNum == 1)
		{
			ChangeAiCharacter();

		}
		
		GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);

		CanChangeAi();
	}), 5.0f, false);
	
}

void AAICharacter::ChangeAiCharacter()
{
	int32 RandomNumber = FMath::RandRange(1, 3);

	ChangeCharacter(RandomNumber);
}

void AAICharacter::ServerAiDie_I()
{
	UCapsuleComponent* AiCapsuleComponent = Cast<UCapsuleComponent>(GetCapsuleComponent());
	AMyAIController* Aicontroller = Cast<AMyAIController>(GetController());

	if (AiCapsuleComponent && AiCapsuleComponent->GetCollisionEnabled() != (ECollisionEnabled::NoCollision))
	{
		isDie = true;
		GetMesh()->SetSimulatePhysics(true);
		AiCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Aicontroller->Destroyed();
		MultiAiDie();
	}
}

bool AAICharacter::ServerAiDie_V()
{
	return true;
}

void AAICharacter::MultiAiDie_Implementation()
{
	DieProcedure();
}
