// Fill out your copyright notice in the Description page of Project Settings.


#include "Tentacle.h"

#include "Components/BoxComponent.h"
#include "Ship.h"

// Sets default values
ATentacle::ATentacle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TentacleBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TentacleBox"));
	RootComponent = TentacleBox;
	TentacleBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	TentacleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TentacleMesh"));
	TentacleMesh->SetupAttachment(TentacleBox);
	TentacleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ATentacle::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = FMath::RandRange(MinHealth, FMath::Max(MinHealth, MaxHealth));
	
	TentacleBox->OnComponentBeginOverlap.AddDynamic(this, &ATentacle::OnTentacleOverlap);
	
	if (SwimAnimation)
	{
		TentacleMesh->PlayAnimation(SwimAnimation, true); // true = en boucle
	}
}

// Called every frame
void ATentacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaTime);
	
	if (FVector::Dist2D(GetActorLocation(), MapCenter) > MapRadius + 500.f)
	{
		Destroy();
	}
}

// Called to bind functionality to input
void ATentacle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
};

bool ATentacle::TakeHit()
{
	CurrentHealth--;

	if (CurrentHealth <= 0)
	{
		Destroy();
		return true;
	}

	StartBlink();
	return false;
}


void ATentacle::StartBlink()
{
	GetWorldTimerManager().ClearTimer(BlinkTimer);
	BlinkCounter = 0;
	TentacleMesh->SetVisibility(true);

	GetWorldTimerManager().SetTimer(
		BlinkTimer, this, &ATentacle::BlinkStep, BlinkInterval, true);
}

void ATentacle::BlinkStep()
{
	BlinkCounter++;
	TentacleMesh->SetVisibility(!TentacleMesh->IsVisible());

	if (BlinkCounter >= BlinkToggles)
	{
		GetWorldTimerManager().ClearTimer(BlinkTimer);
		TentacleMesh->SetVisibility(true);
	}
}

void ATentacle::OnTentacleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AShip* Ship = Cast<AShip>(OtherActor);
	if (!Ship && OtherActor)
	{
		Ship = Cast<AShip>(OtherActor->GetAttachParentActor()); 
	}

	if (Ship)
	{
		Ship->LoseLife();
		Destroy(); 
	}
}