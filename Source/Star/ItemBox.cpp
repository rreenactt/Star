// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"

// Sets default values
AItemBox::AItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
    Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
    Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

    RootComponent = Trigger;
    Box->SetupAttachment(RootComponent);
    Effect->SetupAttachment(RootComponent);

    Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
    if (SM_BOX.Succeeded())
    {
        Box->SetStaticMesh(SM_BOX.Object);
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
    if (P_CHESTOPEN.Succeeded())
    {
        Effect->SetTemplate(P_CHESTOPEN.Object);
        //Effect->bAutoActivate = true;
    }

    Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

    Trigger->SetCollisionProfileName(TEXT("ItemBox"));
    Box->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    //UE_LOG(LogTemp, Warning, TEXT("Test"));
    //Effect->Activate(true);
}

void AItemBox::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnCharacterOverlap);
}

void AItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //여기 부분에 if문으로 캐릭터가 공격중(isAttacking)이 아니면 return하는 구문 필요함
	UE_LOG(LogTemp, Warning, TEXT("Touch"));

    //Effect->Activate(false);
}
