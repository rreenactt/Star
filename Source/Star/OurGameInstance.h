// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OurGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STAR_API UOurGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UOurGameInstance();

	//class FOnlineSessionSettings SessionSettings;

	UFUNCTION(BlueprintCallable)
	void CreateSession(int NumMaxPlayers, bool UseLAN);
};
