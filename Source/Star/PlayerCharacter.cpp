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

	//현재 체력 리플리케이트
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
	
	AddMovementInput(Direction, value);

}

void APlayerCharacter::MoveRight(float value)
{

	// 컨트롤하고 변수에 값이 들어오면..
	if ((Controller != NULL) && value != 0.0f)
	{
		// GetConjtrolRotation을 변수에 가져옴
		const FRotator Rot = Controller->GetControlRotation();
		// 해당 변수에 캐릭터의 Y 값을 가져옴
		const FRotator YawRot(0, Rot.Yaw, 0);
		// 백터값으로 전환해서 저장
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		// 바라보는 방향으로 변수 매개변수 값만큼 이동
		AddMovementInput(Direction, value);
	}
}
// runStart 입력 받는 함수
void APlayerCharacter::RunStart()
{
	isRun = true;
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerPlayerRunStart(isRun);
	}
	PlayerSpeedUpdate();
}

// runStop 입력 받는 함수
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
	// 관한있는지 확인
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
	// 관한있는지 확인
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
	// 각 클라이언트가 가지고 있는 PlayerSpeedUpdate() 호출
	PlayerSpeedUpdate();
}

// 서버에서 각 클라이언트들한테 속도 업데이트하라고 부르는 함수
void APlayerCharacter::PlayerSpeedUpdateCall()
{
	// 멀티케스트 호출
	MultiPlayerSpeedUpdate(isRun);
}

// 클라이언트들의 실질적 속도 변환 함수
void APlayerCharacter::PlayerSpeedUpdate()
{
	//여기서 현재 작동하는 달리기 값 변환중 
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