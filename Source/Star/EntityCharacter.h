// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EntityCharacter.generated.h"



UCLASS()
class STAR_API AEntityCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEntityCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack)
	class UStaticMeshComponent* Weapon;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
	class USkeletalMeshComponent* PlayerMesh;



	// 캐릭터 주머니
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharcaterMesh)
	class USkeletalMesh* Character_Radbit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharcaterMesh)
	class USkeletalMesh* Character_Squirrel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharcaterMesh)
	class USkeletalMesh* Character_Polarbear;

	// 애니메이션 주머니
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimBlueprint)
	class UStaticMesh* Weapon_Radbit;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimBlueprint)
	class UStaticMesh* Weapon_Squirrel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimBlueprint)
	class UStaticMesh* Weapon_Polarbear;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void ChangeCharacter(int32 Number);

	UFUNCTION()
	virtual void Change(int32 Number);

	virtual void Die() PURE_VIRTUAL(AEntityCharacter::Die, );

protected:

	UFUNCTION(Reliable, Server = "Server_CharacterChange_I", WithValidation = "Server_CharacterChange_V")
	virtual void Server_CharacterChange(int32 CharacterNum);
	void Server_CharacterChange_I(int32 CharacterNum);
	bool Server_CharacterChange_V(int32 CharacterNum);

	UFUNCTION(Reliable, NetMulticast)
	virtual void Multi_CharacterChange(int32 CharacterNum);
	void Multi_CharacterChange_Implementation(int32 CharacterNum);
};
