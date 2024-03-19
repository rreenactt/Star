// Fill out your copyright notice in the Description page of Project Settings.


#include "MultyPlayerAnimInstance.h"

UMultyPlayerAnimInstance::UMultyPlayerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Mycontents/PlayerCharacterAM/MyMontage.MyMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UMultyPlayerAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UMultyPlayerAnimInstance::AnimNotify_HitAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Aninotify_HitAttack"));
	HitAttackDelegate.Broadcast();
}

void UMultyPlayerAnimInstance::AnimNotify_EndAttack()
{
	EndAttackDelegate.Broadcast();
}

