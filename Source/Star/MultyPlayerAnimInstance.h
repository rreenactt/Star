// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MultyPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STAR_API UMultyPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	UMultyPlayerAnimInstance();
public:
	void PlayAttackMontage();//¸ùÅ¸ÁÖ ½ÇÇà

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
};
