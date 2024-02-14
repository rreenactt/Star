// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MyPlayerController.h"
#include <Kismet/GameplayStatics.h>

void AMyGameMode::MyGameMode()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();
}

bool AMyGameMode::CountPlayer() const
{
	TArray<AMyPlayerController*> PlayerController;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), PlayerController);
	return false;
}
