// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "MultyPlayerAnimInstance.h"
#include "AICharacter.h"
#include "GameplayGameModeBase.h"
#include "Components/CapsuleComponent.h"
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
	
	// ���� �� �ִ°�
	isAttack = false;

	// ���� �� �� �ִ°�
	isCanAttack = true;

	// �׾��°�
	isDie = false;

	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ���� ������ �浹�ϸ� ������
	Weapon->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnAttackOverlapBegin);
	AnimSeting();

	ChangeCharacter(1);
	
	
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, isRun);
	DOREPLIFETIME(APlayerCharacter, isJump);

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
	if (!isDie)
	{
		// GetConjtrolRotation�� ������ ������
		const FRotator Rot = Controller->GetControlRotation();
		// �ش� ������ ĳ������ Y���� ������
		const FRotator YawRot(0, Rot.Yaw, 0);
		// ���Ͱ����� ��ȯ�ؼ� ����
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
	else
	{
		// GetConjtrolRotation�� ������ ������
		const FRotator Rot = Controller->GetControlRotation();
		// �ش� ������ ĳ������ Y���� ������
		const FRotator YawRot(Rot.Pitch, Rot.Yaw, 0);
		// ���Ͱ����� ��ȯ�ؼ� ����
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);

	}
	

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

void APlayerCharacter::AnimSeting()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ChaingeCharacter!"));

	AnimInstance = Cast<UMultyPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	//OnMontageEnded�� AnimInstance �⺻ �����̴�.��Ÿ�ְ� ������ �� AttackMontageEnded �Լ��� ȣ���Ų��.
	AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnAttackMontageEnded);
	AnimInstance->HitAttackDelegate.AddDynamic(this, &APlayerCharacter::HitAttacking);
	AnimInstance->EndAttackDelegate.AddDynamic(this, &APlayerCharacter::EndAttacking);
	
}


void APlayerCharacter::RunStart()
{
	ServerPlayerRunStart(isRun);
	isRun = true;
	PlayerSpeedUpdate();
}
void APlayerCharacter::RunStop()
{
	ServerPlayerRunStop(isRun);
	
	isRun = false;
	PlayerSpeedUpdate();
}

void APlayerCharacter::JumpStart()
{
	isPlayerJump = true;
	isJump = true;
	ServerPlayerJumpStart(isJump);
	
	PalyerJumpUpdate();
}
void APlayerCharacter::JumpEnd()
{
	isPlayerJump = false;
	isJump = false;
	
	ServerPlayerJumpEnd(isJump);
}

void APlayerCharacter::AttackStart()
{
	// ���ݰ���
	if (!isCanAttack){
		UE_LOG(LogTemp, Warning, TEXT("NOT ATTACK")); 
		return;
	}
	isCanAttack = false;
	
	PalyerAttackUpdate();
	ServerPlayerAttackStart(isAttack);
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			CanAttack();
			UE_LOG(LogTemp, Warning, TEXT("ATTACk_____________END"));

			GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
		}), 3.0f, false);
}

void APlayerCharacter::AttackEnd()
{
	if (isCanAttack) { return; }
	// �̰� �ʿ���� �Լ�
}


void APlayerCharacter::Die()
{
	GetMesh()->SetSimulatePhysics(true);
	CameraBoom->TargetArmLength = 0;
	UCapsuleComponent* MyCapsuleComponent = Cast<UCapsuleComponent>(GetCapsuleComponent());
	MyCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AGameplayGameModeBase* GameMode = Cast<AGameplayGameModeBase>(GetWorld()->GetAuthGameMode());
	AMyPlayerController* MyController = Cast<AMyPlayerController>(GetController());
	if (GameMode && MyController)
	{
		FVector MyVector = MyController->GetPawn()->GetActorLocation();
		FRotator MyRotator = MyController->GetPawn()->GetActorRotation();
		GameMode->PlayerSpectatorconvert(MyController, MyVector, MyRotator);
	}
	isDie = true;
	//ServerDie();
}

void APlayerCharacter::CanAttack()
{
	isCanAttack = true;
}

///////////////////////////////////////////// ĳ���� �ٲٱ�
void APlayerCharacter::CharacterChangeRadbit()
{
	if (isCanAttack)
	{
		isAttack = false;
		ChangeCharacter(1);
		AnimSeting();
	}
}

void APlayerCharacter::CharacterChangeSquirrel()
{
	if (isCanAttack)
	{
		isAttack = false;
		ChangeCharacter(2);
		AnimSeting();
	}
}

void APlayerCharacter::CharacterChangePolarbear()
{
	if (isCanAttack)
	{
		isAttack = false;
		ChangeCharacter(3);
		AnimSeting();
	}
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
	GetCharacterMovement()->MaxWalkSpeed = 375;
	isRun = true;
	
}
bool APlayerCharacter::ServerPlayerRunStart_V(bool re)
{
	return true;
}
// Server Run Stop
void APlayerCharacter::ServerPlayerRunStop_I(bool run)
{
	GetCharacterMovement()->MaxWalkSpeed = 125;
	isRun = false;
	
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
// ��������Ʈ ȣ���Լ�
void APlayerCharacter::HitAttacking()
{
	UE_LOG(LogTemp, Warning, TEXT("isAttack---------true"));
	isAttack = true;
}
void APlayerCharacter::EndAttacking()
{
	UE_LOG(LogTemp, Warning, TEXT("isAttack-----------false"));
	isAttack = false;
}

void APlayerCharacter::ServerPlayerAttackStart_I(bool att)
{
	AnimInstance = Cast<UMultyPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	// ���н� ����
	if (nullptr == AnimInstance){ return; }
	AnimInstance->PlayAttackMontage();
	MultiPlayerAttackUpdate();
}
bool APlayerCharacter::ServerPlayerAttackStart_V(bool att)
{
	return true;
}

void APlayerCharacter::MultiPlayerAttackUpdate_Implementation()
{
	PalyerAttackUpdate();
}
void APlayerCharacter::PalyerAttackUpdate()
{
	AnimInstance = Cast<UMultyPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	// ���н� ����
	if (nullptr == AnimInstance){ return; }
	// ��Ÿ�� �÷���
	AnimInstance->PlayAttackMontage();
}
void APlayerCharacter::OnAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!(OtherActor->GetClass()->IsChildOf(AEntityCharacter::StaticClass())) || OtherActor == this || !isAttack)
	{
		return;
	}
	
	// AI
	Target = Cast<AAICharacter>(OtherActor);
	if (Target)
	{
		Target->Die();
		ServerPlayerToKill(OtherActor);
	}
	else
	{
		// ��� �÷��̾�
		Target = Cast<APlayerCharacter>(OtherActor);
		if (Target)
		{
			Target->Die();
			ServerPlayerToKill(OtherActor);
		}
	}
	
}
void APlayerCharacter::ServerDie_I()
{
	
	GetMesh()->SetSimulatePhysics(true);
	UCapsuleComponent* MyCapsuleComponent = Cast<UCapsuleComponent>(GetCapsuleComponent());
	MyCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	
	AGameplayGameModeBase* GameMode = Cast<AGameplayGameModeBase>(GetWorld()->GetAuthGameMode());
	AMyPlayerController* MyController = Cast<AMyPlayerController>(GetController());
	if (GameMode && MyController)
	{
		FVector MyVector = MyController->GetPawn()->GetActorLocation();
		FRotator MyRotator = MyController->GetPawn()->GetActorRotation();
		GameMode->PlayerSpectatorconvert(MyController, MyVector, MyRotator);
	}

	isDie = true;
	PlayerDieCall();
}
bool APlayerCharacter::ServerDie_V()
{
	return true;
}
void APlayerCharacter::MultiKill_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	UCapsuleComponent* MyCapsuleComponent = Cast<UCapsuleComponent>(GetCapsuleComponent());
	MyCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AGameplayGameModeBase* GameMode = Cast<AGameplayGameModeBase>(GetWorld()->GetAuthGameMode());
	AMyPlayerController* MyController = Cast<AMyPlayerController>(GetController());
	if (GameMode && MyController)
	{
		FVector MyVector = MyController->GetPawn()->GetActorLocation();
		FRotator MyRotator = MyController->GetPawn()->GetActorRotation();
		GameMode->PlayerSpectatorconvert(MyController, MyVector, MyRotator);
	}

	isDie = true;

}

void APlayerCharacter::ServerPlayerToKill_I(AActor* OtherActor)
{
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
	MultiPlayerToKill(OtherActor);
}

bool APlayerCharacter::ServerPlayerToKill_V(AActor* OtherActor)
{
	return true;
}

void APlayerCharacter::MultiPlayerToKill_Implementation(AActor* OtherActor)
{
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
