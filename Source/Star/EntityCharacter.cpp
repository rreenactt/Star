// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEntityCharacter::AEntityCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	FName WeaponSocket(TEXT("RightHandIndex2"));

    // �䳢 �޽�
        static ConstructorHelpers::FObjectFinder<USkeletalMesh> RadbitMesh(TEXT("/Game/Mycontents/Character/Rabbit/Rabbit_Character_Rigged_.Rabbit_Character_Rigged_"));
    if (RadbitMesh.Succeeded())
    {
        Character_Radbit = RadbitMesh.Object;
    }
    // �ٶ��� �޽�
        static ConstructorHelpers::FObjectFinder<USkeletalMesh> SquirrelMesh(TEXT("/Game/Mycontents/Character/Squirrel/Squirrel__Rigged__.Squirrel__Rigged__"));
    if (SquirrelMesh.Succeeded())
    {
        Character_Squirrel = SquirrelMesh.Object;
    }
    // �ϱذ� �޽�
        static ConstructorHelpers::FObjectFinder<USkeletalMesh> PolarBearMesh(TEXT("/Game/Mycontents/Character/PolarBear/PolarBear_Rigged_.PolarBear_Rigged_"));
    if (PolarBearMesh.Succeeded())
    {
        Character_Polarbear = PolarBearMesh.Object;
    }
    // �䳢����
        static ConstructorHelpers::FObjectFinder<UStaticMesh> RadbitWeapon(TEXT("/Game/Mycontents/Character/Weapon/Carrot_Knife/Carrot_Knife_.Carrot_Knife_"));
    if (RadbitWeapon.Succeeded())
    {
        Weapon_Radbit = RadbitWeapon.Object;
    }
    // �ٶ��� ����
        static ConstructorHelpers::FObjectFinder<UStaticMesh> SquirrelWeapon(TEXT("/Game/Mycontents/Character/Weapon/acorn_Knife/acornKnife.acornKnife"));
    if (SquirrelWeapon.Succeeded())
    {
        Weapon_Squirrel = SquirrelWeapon.Object;
    }
    // �ϱذ� ����
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PolarBearWeapon(TEXT("/Game/Mycontents/Character/Weapon/tuna_Knife/tunaKnife.tunaKnife"));
    if (PolarBearWeapon.Succeeded())
    {
        Weapon_Polarbear = PolarBearWeapon.Object;
    }

    Weapon->SetStaticMesh(Weapon_Squirrel);
    Weapon->SetupAttachment(GetMesh(), WeaponSocket);
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
void AEntityCharacter::Change(int32 Number)
{
    int32 CharNum = Number;

    switch (CharNum) {
    case 1:
        GetMesh()->SetSkeletalMeshAsset(Character_Radbit);
        Weapon->SetStaticMesh(Weapon_Radbit);
        Weapon->SetupAttachment(GetMesh(), TEXT("RightHandIndex2"));
        break;
    case 2:
        GetMesh()->SetSkeletalMeshAsset(Character_Squirrel);
        Weapon->SetStaticMesh(Weapon_Squirrel);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ChaingeCharacter!"));
        break;
    case 3:
        GetMesh()->SetSkeletalMeshAsset(Character_Polarbear);
        Weapon->SetStaticMesh(Weapon_Polarbear);
        break;
    default:
        return;
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
        GetMesh()->SetSkeletalMeshAsset(Character_Radbit);
        Weapon->SetStaticMesh(Weapon_Radbit);
        Weapon->SetupAttachment(GetMesh(), TEXT("RightHandIndex2"));
        break;
    case 2:
        GetMesh()->SetSkeletalMeshAsset(Character_Squirrel);
        Weapon->SetStaticMesh(Weapon_Squirrel);
        break;
    case 3:
        GetMesh()->SetSkeletalMeshAsset(Character_Polarbear);
        Weapon->SetStaticMesh(Weapon_Polarbear);
        break;
    default:
        return;
    }
    Multi_CharacterChange(CharacterNum);
}

bool AEntityCharacter::Server_CharacterChange_V(int32 CharacterNum)
{
    return true;
}

void AEntityCharacter::Multi_CharacterChange_Implementation(int32 CharacterNum)
{

    Change(CharacterNum);
}

