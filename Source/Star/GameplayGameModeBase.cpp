// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayGameModeBase.h"
#include "MyPlayerController.h"
#include "PlayerSpectatorPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpectatorPawn.h"


AGameplayGameModeBase::AGameplayGameModeBase()
{
}
void AGameplayGameModeBase::BeginPlay()
{
}

void AGameplayGameModeBase::PlayerSpectatorconvert(AMyPlayerController* PlayerController, FVector CameraLocation, FRotator CameraRotator)
{
    UE_LOG(LogTemp, Warning, TEXT("In GameMode"));

	if (PlayerController)
	{
		PlayerController->UnPossess();
        ASpectatorPawn* NewSpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), CameraLocation, CameraRotator);
        if (NewSpectatorPawn)
        {
            UE_LOG(LogTemp, Warning, TEXT("last"));
            // 관전자 컨트롤 설정
            PlayerController->Possess(NewSpectatorPawn);
            PlayerController->SetViewTargetWithBlend(NewSpectatorPawn, 0.5f);
        }
	}
}
