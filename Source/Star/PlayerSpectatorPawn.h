// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "PlayerSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class STAR_API APlayerSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:
	APlayerSpectatorPawn();

//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
//	UCapsuleComponent CapsuleComponent; // CapsuleComponent ���� 

};