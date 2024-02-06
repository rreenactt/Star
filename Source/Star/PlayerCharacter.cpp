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

	// 카메라 암 생성
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// 캐릭터에 카메라암 붙임
	CameraBoom->SetupAttachment(RootComponent);
	// 카메라암 길이 조정
	CameraBoom->TargetArmLength = 850.0f;
	// 카메라 암 회전 허용
	CameraBoom->bUsePawnControlRotation = true;
	// 카메라 보는 쪽으로 회전 허용
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 회전값 카메라가 보는 곳으로
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	// 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// 카메라 붙이기
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// 카메라만 회전 불허
	FollowCamera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	//RunSpeed = GetCharacterMovement()->MaxWalkSpeed;
	//GetCharacterMovement()->SetIsReplicated(true);
	RunSpeed = 600.0f;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//현재 체력 리플리케이트
	DOREPLIFETIME(APlayerCharacter, RunSpeed);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 앞뒤좌우 움직이기
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// 카메라 방향으로 시점 전환
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);

	// 캐릭터 달리기 시작과 끝
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::RunStart);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::RunStop);

}


void APlayerCharacter::MoveForward(float value)
{
	// GetConjtrolRotation을 변수에 가져옴
	const FRotator Rot = Controller->GetControlRotation();

	// 해당 변수에 캐릭터의 Y값을 가져옴
	const FRotator YawRot(0, Rot.Yaw, 0);
	// 백터값으로 전환해서 저장
	const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	RunSpeed *= value;
	AddMovementInput(Direction, RunSpeed);

}

void APlayerCharacter::MoveRight(float value)
{

	// 컨트롤하고 변수에 값이 들어오면..
	if ((Controller != NULL) && value != 0.0f)
	{
		// GetConjtrolRotation을 변수에 가져옴
		const FRotator Rot = Controller->GetControlRotation();
		// 해당 변수에 캐릭터의 Y값을 가져옴
		const FRotator YawRot(0, Rot.Yaw, 0);
		// 백터값으로 전환해서 저장
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		RunSpeed *= value;
		// 바라보는 방향으로 변수 매개변수 값만큼 이동
		AddMovementInput(Direction, RunSpeed);
	}
}
void APlayerCharacter::RunStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Start"));
	//GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	RunSpeed = 1200.0f;
	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);

}
void APlayerCharacter::RunStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop"));
	RunSpeed = 600.0f;

	//GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);
}

// 클라이언트 이동속도 업데이트 함수
void APlayerCharacter::ClientUpdateWalkSpeed_Implementation(float RunSpeed)
{
}
void APlayerCharacter::ClientUpdateWalkSpeed_Validate(float RunSpeed)
{

}
// 서버 이동속도 업데이트 함수 
void APlayerCharacter::ServerUpdateWalkSpeed_Implementation(float RunSpeed)
{

}
bool APlayerCharacter::ServerUpdateWalkSpeed_Validate()
{

}
