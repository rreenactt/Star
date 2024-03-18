// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MultyPlayerAnimInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHitAttackCheckDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndAttackDelegate);
/**
 * 
 */


UCLASS()
class STAR_API UMultyPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	
	UMultyPlayerAnimInstance();
public:

	FHitAttackCheckDelegate HitAttackDelegate;
	FEndAttackDelegate EndAttackDelegate;

	void PlayAttackMontage();//¸ùÅ¸ÁÖ ½ÇÇà

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UFUNCTION()
	void AnimNotify_HitAttack();
	UFUNCTION()
	void AnimNotify_EndAttack();
};
