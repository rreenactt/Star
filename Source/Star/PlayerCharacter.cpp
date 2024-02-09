// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ī�޶� �� ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// ĳ���Ϳ� ī�޶�� ����
	CameraBoom->SetupAttachment(RootComponent);
	// ī�޶�� ���� ����
	CameraBoom->TargetArmLength = 850.0f;
	// ī�޶� �� ȸ�� ���
	CameraBoom->bUsePawnControlRotation = true;
	// ī�޶� ���� ������ ȸ�� ���
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// ȸ���� ī�޶� ���� ������
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	// ī�޶� ����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// ī�޶� ���̱�
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// ī�޶� ȸ�� ����
	FollowCamera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	
	bReplicates = true;

	isRun = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//���� ü�� ���ø�����Ʈ
	DOREPLIFETIME(APlayerCharacter, isRun);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �յ��¿� �����̱�
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// ī�޶� �������� ���� ��ȯ
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);

	// ĳ���� �޸��� ���۰� ��
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::RunStart);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::RunStop);

}


void APlayerCharacter::MoveForward(float value)
{
	// GetConjtrolRotation�� ������ ������
	const FRotator Rot = Controller->GetControlRotation();

	// �ش� ������ ĳ������ Y���� ������
	const FRotator YawRot(0, Rot.Yaw, 0);
	// ���Ͱ����� ��ȯ�ؼ� ����
	const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	
	AddMovementInput(Direction, value);

}

void APlayerCharacter::MoveRight(float value)
{

	// ��Ʈ���ϰ� ������ ���� ������..
	if ((Controller != NULL) && value != 0.0f)
	{
		// GetConjtrolRotation�� ������ ������
		const FRotator Rot = Controller->GetControlRotation();
		// �ش� ������ ĳ������ Y���� ������
		const FRotator YawRot(0, Rot.Yaw, 0);
		// ���Ͱ����� ��ȯ�ؼ� ����
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		// �ٶ󺸴� �������� ���� �Ű����� ����ŭ �̵�
		AddMovementInput(Direction, value);
	}
}
// runStart �Է� �޴� �Լ�
void APlayerCharacter::RunStart()
{
	isRun = true;
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerPlayerRunStart(isRun);
	}
	PlayerSpeedUpdate();
}

// runStop �Է� �޴� �Լ�
void APlayerCharacter::RunStop()
{
	isRun = false;
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerPlayerRunStop(isRun);
	}
	PlayerSpeedUpdate();
}

// Server Run Start
void APlayerCharacter::ServerPlayerRunStart_I(bool run)
{
	// �����ִ��� Ȯ��
	if (HasAuthority())
	{
		GetCharacterMovement()->MaxWalkSpeed = 750;
		isRun = true;
	}
}
bool APlayerCharacter::ServerPlayerRunStart_V(bool re)
{
	return true;
}


// Server Run Stop
void APlayerCharacter::ServerPlayerRunStop_I(bool run)
{
	// �����ִ��� Ȯ��
	if (HasAuthority())
	{
		GetCharacterMovement()->MaxWalkSpeed = 330;
		isRun = false;
	}
}
bool APlayerCharacter::ServerPlayerRunStop_V(bool re)
{
	return true;
}

// Multicast
void APlayerCharacter::MultiPlayerSpeedUpdate_Implementation(bool run)
{
	// �� Ŭ���̾�Ʈ�� ������ �ִ� PlayerSpeedUpdate() ȣ��
	PlayerSpeedUpdate();
}

// �������� �� Ŭ���̾�Ʈ������ �ӵ� ������Ʈ�϶�� �θ��� �Լ�
void APlayerCharacter::PlayerSpeedUpdateCall()
{
	// ��Ƽ�ɽ�Ʈ ȣ��
	MultiPlayerSpeedUpdate(isRun);
}

// Ŭ���̾�Ʈ���� ������ �ӵ� ��ȯ �Լ�
void APlayerCharacter::PlayerSpeedUpdate()
{
	//���⼭ ���� �۵��ϴ� �޸��� �� ��ȯ�� 
	if (isRun)
	{
		UE_LOG(LogTemp, Warning, TEXT("set speed 1200"));

		GetCharacterMovement()->MaxWalkSpeed = 750;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("set speed 500"));

		GetCharacterMovement()->MaxWalkSpeed = 330;
	}
}