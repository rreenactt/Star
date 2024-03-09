// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayGameModeBase.h"
#include "MyPlayerController.h"
#include "GameFramework/SpectatorPawn.h"


AGameplayGameModeBase::AGameplayGameModeBase()
{
}
void AGameplayGameModeBase::BeginPlay()
{
}

//void AGameplayGameModeBase::PlayerSpectatorconvert(AMyPlayerController* PlayerController, FVector CameraLocation, FRotator CameraRotator, Camera)
//{
//	if (PlayerController)
//	{
//		PlayerController->UnPossess();
//        ASpectatorPawn* NewSpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), CameraLocation, CameraRotator);
//        if (NewSpectatorPawn)
//        {
//            APlayerCameraManager* PlayerCameraManager = PlayerController->;
//            if (PlayerCameraManager)
//            {
//                // 관전자 컨트롤 설정
//                PlayerController->Possess(NewSpectatorPawn);
//                PlayerCameraManager->SetViewTargetWithBlend(NewSpectatorPawn, 0.5f);
//            }
//        }
//	}
//}
