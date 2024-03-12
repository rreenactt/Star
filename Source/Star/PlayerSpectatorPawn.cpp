// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpectatorPawn.h"
#include "Components/CapsuleComponent.h"


APlayerSpectatorPawn::APlayerSpectatorPawn()
{
    // Create the capsule component
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent")); // CapsuleComponent 생성
    CapsuleComponent->CapsuleComponent(RootComponent);
    // Set up collision settings for the capsule component
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
    CapsuleComponent->SetCollisionObjectType(ECC_WorldStatic); // 충돌 오브젝트 타입 설정
    CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore); // 모든 채널에 대한 충돌 응답 무시
    CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // ECC_WorldStatic 채널에 대한 충돌 응답 블록


}
