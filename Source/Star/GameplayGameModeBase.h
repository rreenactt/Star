// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPlayerController.h"
#include "GameplayGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class STAR_API AGameplayGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameplayGameModeBase();

	virtual void BeginPlay() override;

	void PlayerSpectatorconvert(AMyPlayerController* PlayerController, FVector CameraLocation, FRotator CameraRotator);
};
