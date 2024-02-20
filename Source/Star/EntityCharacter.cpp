// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEntityCharacter::AEntityCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    PlayerMesh->SetupAttachment(RootComponent);


	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	FName WeaponSocket(TEXT("RightHandIndex2"));
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	Weapon->SetupAttachment(PlayerMesh, WeaponSocket);

    // �䳢 �޽�
        static ConstructorHelpers::FObjectFinder<USkeletalMeshComponent> RadbitMesh(TEXT("/Game/Mycontents/PlayerCharacterAM/MyMontage.MyMontage"));//�ȴ�
    if (RadbitMesh.Succeeded())
    {
        Character_Radbit = RadbitMesh.Object;
    }
    // �ٶ��� �޽�
        static ConstructorHelpers::FObjectFinder<USkeletalMeshComponent> SquirrelMesh(TEXT("/Game/Mycontents/PlayerCharacterAM/MyMontage.MyMontage"));// �ȴ�
    if (SquirrelMesh.Succeeded())
    {
        Character_Radbit = SquirrelMesh.Object;
    }
    // �ϱذ� �޽�
        static ConstructorHelpers::FObjectFinder<USkeletalMeshComponent> PolarBearMesh(TEXT("/Game/Mycontents/PlayerCharacterAM/MyMontage.MyMontage"));// �ȴ�
    if (PolarBearMesh.Succeeded())
    {
        Character_Polarbear = PolarBearMesh.Object;
    }
    // �䳢 �ִϸ��̼��ν��Ͻ�
        static ConstructorHelpers::FObjectFinder<UStaticMeshComponent> RadbitWeapon(TEXT("/Game/Mycontents/PlayerCharacterAM/MyMontage.MyMontage"));// �ȴ�
    if (RadbitWeapon.Succeeded())
    {
        Weapon_Radbit = RadbitWeapon.Object;
    }
    // �ٶ��� �ִϸ��̼��ν��Ͻ�
        static ConstructorHelpers::FObjectFinder<UStaticMeshComponent> SquirrelWeapon(TEXT("/Game/Mycontents/PlayerCharacterAM/MyMontage.MyMontage"));// �ȴ�
    if (SquirrelWeapon.Succeeded())
    {
        Weapon_Squirrel = SquirrelWeapon.Object;
    }
    // �ٶ��� �ִϸ��̼��ν��Ͻ�
    static ConstructorHelpers::FObjectFinder<UStaticMeshComponent> PolarBearWeapon(TEXT("/Game/Mycontents/PlayerCharacterAM/MyMontage.MyMontage"));// �ȴ�
    if (PolarBearWeapon.Succeeded())
    {
        Weapon_Polarbear = PolarBearWeapon.Object;
    }
}

// Called when the game starts or when spawned
void AEntityCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEntityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEntityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AEntityCharacter::ChangeCharacter(int32 Number)
{
    if (HasAuthority()) // ���� �ν��Ͻ��� �������� Ȯ��
    {
        // �������� ���� ����
        Server_CharacterChange(Number);
        
    }
    else if (GetLocalRole() == ROLE_AutonomousProxy) // ���� �ν��Ͻ��� Ŭ���̾�Ʈ���� Ȯ��
    {
        // ������ ȣ�� ��û
        Server_CharacterChange(Number);

    }
   
}
/// <summary>
/// ////////////////////////////////////////////////////////////////////// ù��° ĳ���� 
/// </summary>
void AEntityCharacter::Server_CharacterChange_I(int32 CharacterNum)
{
    int32 CharNum = CharacterNum;

    switch (CharNum) {
    case 1:
        
        break;
    case 2:

        break;
    case 3:

        break;
    default:
        return;
    }
}

bool AEntityCharacter::Server_CharacterChange_V(int32 CharacterNum)
{
    return true;
}

void AEntityCharacter::Multi_CharacterChange_Implementation(int32 CharacterNum)
{
}

