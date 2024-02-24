// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "MultyPlayerAnimInstance.h"
#include "AICharacter.h"
#include "Components/StaticMeshComponent.h"
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
	
	// 리플리케이션 허용
	bReplicates = true;
	// 달리고 있는가
	isRun = false;
	// 공중에 있는가
	isFalling = false;
	// 점프를 했는가(서버)
	isJump = false;
	// 점프를 했는가(플레이어)
	isPlayerJump = false;
	// 공격을 했는가
	isAttack = false;
	// 공격 중인가
	isAttacking = false;

	// 공격 할 수 있는가
	isCanAttack = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 공격 도구가 충돌하면 보내기
	Weapon->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnAttackOverlapBegin);
	auto AnimInstance = Cast<UMultyPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	//OnMontageEnded는 AnimInstance 기본 변수이다.몽타주가 끝났을 때 AttackMontageEnded 함수를 호출시킨다.
	AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnAttackMontageEnded);
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//현재 체력 리플리케이트
	DOREPLIFETIME(APlayerCharacter, isRun);
	DOREPLIFETIME(APlayerCharacter, isJump);
	DOREPLIFETIME(APlayerCharacter, isAttack);
	DOREPLIFETIME(APlayerCharacter, isAttacking);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	isFalling = GetCharacterMovement()->IsFalling();

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

	// 캐릭터 점프 시작과 끝
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpStart);

	// 공격
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::AttackStart);

	// 변신
	PlayerInputComponent->BindAction("Radbit", IE_Pressed, this, &APlayerCharacter::CharacterChangeRadbit);
	PlayerInputComponent->BindAction("Squirrel", IE_Pressed, this, &APlayerCharacter::CharacterChangeSquirrel);
	PlayerInputComponent->BindAction("Polarbear", IE_Pressed, this, &APlayerCharacter::CharacterChangePolarbear);


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
void APlayerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AttackEnd();
}
// 착지, 지면에 닿으면 호출되는 함수
void APlayerCharacter::Landed(const FHitResult& Hit)
{
	JumpEnd();
}


void APlayerCharacter::RunStart()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerPlayerRunStart(isRun);
	}
	isRun = true;
	PlayerSpeedUpdate();
}
void APlayerCharacter::RunStop()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerPlayerRunStop(isRun);
	}
	isRun = false;
	PlayerSpeedUpdate();
}

void APlayerCharacter::JumpStart()
{
	isPlayerJump = true;
	isJump = true;
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerPlayerJumpStart(isJump);
	}
	PalyerJumpUpdate();
}
void APlayerCharacter::JumpEnd()
{
	isPlayerJump = false;
	isJump = false;
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerPlayerJumpEnd(isJump);
	}
}

void APlayerCharacter::AttackStart()
{
	if (!isCanAttack)
	{
		return;
	}
	if (GetLocalRole() < ROLE_Authority && !isAttack && !isAttacking)
	{
		ServerPlayerAttackStart(isAttack);
	}
	isCanAttack = false;
	isAttacking = true;
	isAttack = true;
	PalyerAttackUpdate();
}

void APlayerCharacter::AttackEnd()
{
	if (GetLocalRole() < ROLE_Authority && isAttack)
	{
		ServerPlayerAttackEnd(isAttack);
	}
	isAttacking = false;
	isAttack = false;
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
			CanAttack();

	GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
	}), 1.2f, false);
}


void APlayerCharacter::Die()
{
	GetMesh()->SetSimulatePhysics(true);
	ServerKill();
}

void APlayerCharacter::CanAttack()
{
	isCanAttack = true;
}

///////////////////////////////////////////// 캐릭터 바꾸기
void APlayerCharacter::CharacterChangeRadbit()
{
	ChangeCharacter(1);
}

void APlayerCharacter::CharacterChangeSquirrel()
{
	ChangeCharacter(2);
}

void APlayerCharacter::CharacterChangePolarbear()
{
	ChangeCharacter(3);
}
/////////////////////////////////////////////////////////////// 기능 구현 부분

// 변수가 변했을때 호출하는 함수 부분
// speed update call 
void APlayerCharacter::PlayerSpeedUpdateCall() 
{
	MultiPlayerSpeedUpdate(); // 멀티케스트 호출	
} 
//Jump Update call
void APlayerCharacter::PlayerJumpUpdateCall()
{
	MultiPlayerJumpUpdate(); // 멀티케스트 호출
}
//Attack Update call
void APlayerCharacter::PlayerAttackUpdateCall()
{
	MultiPlayerAttackUpdate();
}

void APlayerCharacter::PlayerDieCall()
{
	MultiKill();
}

/////////////////////////////////////////////////////////////// RUN

// Run server 동기화
// Server Run Start
void APlayerCharacter::ServerPlayerRunStart_I(bool run)
{
	// 관한있는지 확인
	if (HasAuthority())
	{
		GetCharacterMovement()->MaxWalkSpeed = 375;
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
		GetCharacterMovement()->MaxWalkSpeed = 125;
		isRun = false;
	}
}
bool APlayerCharacter::ServerPlayerRunStop_V(bool re)
{
	return true;
}
// Multicast
void APlayerCharacter::MultiPlayerSpeedUpdate_Implementation()
{
	// 각 클라이언트가 가지고 있는 PlayerSpeedUpdate() 호출
	PlayerSpeedUpdate();
}
// 서버에서 각 클라이언트들한테 속도 업데이트하라고 부르는 함수
// 클라이언트들의 실질적 속도 변환 함수
void APlayerCharacter::PlayerSpeedUpdate()
{
	//여기서 현재 작동하는 달리기 값 변환중 
	if (isRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = 375;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 125;
	}
} 

/////////////////////////////////////////////////////////////// JUMP

// Jump server 동기화
// Jump Start
void APlayerCharacter::ServerPlayerJumpStart_I(bool jump)
{
	if (HasAuthority())
	{
		Super::Jump();
		isPlayerJump = true;
		isJump = true;
	}
}
bool APlayerCharacter::ServerPlayerJumpStart_V(bool jum)
{
	return true;
}
// 착지
void APlayerCharacter::ServerPlayerJumpEnd_I(bool jump)
{
	if (HasAuthority())
	{
		isPlayerJump = false;
		isJump = false;
	}
}
bool APlayerCharacter::ServerPlayerJumpEnd_V(bool jum)
{
	return true;
}
void APlayerCharacter::MultiPlayerJumpUpdate_Implementation()
{
	// 각클라이언트들 JumpUpdate 함수 호출
	PalyerJumpUpdate();
}
void APlayerCharacter::PalyerJumpUpdate()
{
	if (isFalling)
	{
		return;
	}
	if (!isJump)
	{
		isPlayerJump = false;
		return;
	}
	if (isJump)
	{
		isPlayerJump = true;
		Super::Jump();
	}
}


/////////////////////////////////////////////////////////////// ATTACK

void APlayerCharacter::ServerPlayerAttackStart_I(bool att)
{
	auto AnimInstance = Cast<UMultyPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	// 실패시 리턴
	if (nullptr == AnimInstance)
		return;

	AnimInstance->PlayAttackMontage();
	isAttack = true;
	isAttacking = true;

}
bool APlayerCharacter::ServerPlayerAttackStart_V(bool att)
{
	return true;
}
void APlayerCharacter::ServerPlayerAttackEnd_I(bool att)
{
	isAttack = false;
	isAttacking = false;
}
bool APlayerCharacter::ServerPlayerAttackEnd_V(bool att)
{
	return true;
}
void APlayerCharacter::MultiPlayerAttackUpdate_Implementation()
{
	PalyerAttackUpdate();
}
void APlayerCharacter::PalyerAttackUpdate()
{
	if (isAttack)
	{
		auto AnimInstance = Cast<UMultyPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		// 실패시 리턴
		if (nullptr == AnimInstance)
			return;

		AnimInstance->PlayAttackMontage();
	}
	else
	{
		isAttacking = false;
		isAttack = false;
	}

	
}
void APlayerCharacter::OnAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!(OtherActor->GetClass()->IsChildOf(AEntityCharacter::StaticClass())) || OtherActor == this && OtherActor == NULL || !isAttacking || OtherActor == this)
	{
		return;
	}

	// AI
	Target = Cast<AAICharacter>(OtherActor);
	if (Target)
	{
		Target->Die();
	}
	else
	{
		// 상대 플레이어
		Target = Cast<APlayerCharacter>(OtherActor);
		if (Target)
		{
			Target->Die();
		}
	}
}
void APlayerCharacter::ServerKill_I()
{
	GetMesh()->SetSimulatePhysics(true);
	PlayerDieCall();
}
bool APlayerCharacter::ServerKill_V()
{
	return true;
}
void APlayerCharacter::MultiKill_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
}