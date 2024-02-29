// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainmenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class STAR_API AMainmenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CreateSession(FString SessionName, bool IsLan);
};
