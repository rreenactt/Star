// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STAR_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
	
	void MyGameMode();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	bool CountPlayer() const;
};
