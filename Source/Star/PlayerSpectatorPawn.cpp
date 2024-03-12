// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpectatorPawn.h"
#include "Components/CapsuleComponent.h"


APlayerSpectatorPawn::APlayerSpectatorPawn()
{
    // Create the capsule component
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent")); // CapsuleComponent ����
    CapsuleComponent->CapsuleComponent(RootComponent);
    // Set up collision settings for the capsule component
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �浹 Ȱ��ȭ
    CapsuleComponent->SetCollisionObjectType(ECC_WorldStatic); // �浹 ������Ʈ Ÿ�� ����
    CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore); // ��� ä�ο� ���� �浹 ���� ����
    CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // ECC_WorldStatic ä�ο� ���� �浹 ���� ���


}
