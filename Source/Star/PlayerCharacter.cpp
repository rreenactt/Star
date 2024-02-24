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
	
	// ���ø����̼� ���
	bReplicates = true;
	// �޸��� �ִ°�
	isRun = false;
	// ���߿� �ִ°�
	isFalling = false;
	// ������ �ߴ°�(����)
	isJump = false;
	// ������ �ߴ°�(�÷��̾�)
	isPlayerJump = false;
	// ������ �ߴ°�
	isAttack = false;
	// ���� ���ΰ�
	isAttacking = false;

	// ���� �� �� �ִ°�
	isCanAttack = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ���� ������ �浹�ϸ� ������
	Weapon->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnAttackOverlapBegin);
	auto AnimInstance = Cast<UMultyPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	//OnMontageEnded�� AnimInstance �⺻ �����̴�.��Ÿ�ְ� ������ �� AttackMontageEnded �Լ��� ȣ���Ų��.
	AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnAttackMontageEnded);
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//���� ü�� ���ø�����Ʈ
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

	// �յ��¿� �����̱�
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// ī�޶� �������� ���� ��ȯ
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);

	// ĳ���� �޸��� ���۰� ��
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::RunStart);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::RunStop);

	// ĳ���� ���� ���۰� ��
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpStart);

	// ����
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::AttackStart);

	// ����
	PlayerInputComponent->BindAction("Radbit", IE_Pressed, this, &APlayerCharacter::CharacterChangeRadbit);
	PlayerInputComponent->BindAction("Squirrel", IE_Pressed, this, &APlayerCharacter::CharacterChangeSquirrel);
	PlayerInputComponent->BindAction("Polarbear", IE_Pressed, this, &APlayerCharacter::CharacterChangePolarbear);


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
		// �ش� ������ ĳ������ Y ���� ������
		const FRotator YawRot(0, Rot.Yaw, 0);
		// ���Ͱ����� ��ȯ�ؼ� ����
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		// �ٶ󺸴� �������� ���� �Ű����� ����ŭ �̵�
		AddMovementInput(Direction, value);
	}
}
void APlayerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AttackEnd();
}
// ����, ���鿡 ������ ȣ��Ǵ� �Լ�
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

///////////////////////////////////////////// ĳ���� �ٲٱ�
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
/////////////////////////////////////////////////////////////// ��� ���� �κ�

// ������ �������� ȣ���ϴ� �Լ� �κ�
// speed update call 
void APlayerCharacter::PlayerSpeedUpdateCall() 
{
	MultiPlayerSpeedUpdate(); // ��Ƽ�ɽ�Ʈ ȣ��	
} 
//Jump Update call
void APlayerCharacter::PlayerJumpUpdateCall()
{
	MultiPlayerJumpUpdate(); // ��Ƽ�ɽ�Ʈ ȣ��
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

// Run server ����ȭ
// Server Run Start
void APlayerCharacter::ServerPlayerRunStart_I(bool run)
{
	// �����ִ��� Ȯ��
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
	// �����ִ��� Ȯ��
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
	// �� Ŭ���̾�Ʈ�� ������ �ִ� PlayerSpeedUpdate() ȣ��
	PlayerSpeedUpdate();
}
// �������� �� Ŭ���̾�Ʈ������ �ӵ� ������Ʈ�϶�� �θ��� �Լ�
// Ŭ���̾�Ʈ���� ������ �ӵ� ��ȯ �Լ�
void APlayerCharacter::PlayerSpeedUpdate()
{
	//���⼭ ���� �۵��ϴ� �޸��� �� ��ȯ�� 
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

// Jump server ����ȭ
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
// ����
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
	// ��Ŭ���̾�Ʈ�� JumpUpdate �Լ� ȣ��
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
	// ���н� ����
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
		// ���н� ����
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
		// ��� �÷��̾�
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