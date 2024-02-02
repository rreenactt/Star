// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
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
	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);



	RunSpeed = 2.0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
void APlayerCharacter::RunStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Start"));
	GetCharacterMovement()-> MaxWalkSpeed *= RunSpeed;
	UE_LOG(LogTemp, Warning, TEXT("%d"), GetCharacterMovement()->MaxWalkSpeed);

}
void APlayerCharacter::RunStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop"));

	GetCharacterMovement()-> MaxWalkSpeed /= RunSpeed;

}
