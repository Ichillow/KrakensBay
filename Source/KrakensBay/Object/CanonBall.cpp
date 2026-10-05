// Fill out your copyright notice in the Description page of Project Settings.


#include "CanonBall.h"

#include "NiagaraFunctionLibrary.h"
#include "Ship.h"
#include "Tentacle.h"

// Sets default values
ACanonBall::ACanonBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(15.f);
	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = Collision;

	CanonBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	CanonBallMesh->SetupAttachment(Collision);
	CanonBallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 3000.f;
	Movement->MaxSpeed = 3000.f;
	Movement->ProjectileGravityScale = 1.f;
	Movement->bRotationFollowsVelocity = true;

	InitialLifeSpan = 5.f;

}

// Called when the game starts or when spawned
void ACanonBall::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACanonBall::OnBallOverlap);
}

// Called every frame
void ACanonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetActorLocation().Z <= WaterHeight)
	{
		// Spawn Niagara effect at the location of the canon ball
		if (SplashFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SplashFX, GetActorLocation());
		}
		Destroy();
	}
}

void ACanonBall::OnBallOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATentacle* Tentacle = Cast<ATentacle>(OtherActor))
	{
		const int32 Points = Tentacle->ScoreValue;

		if (Tentacle->TakeHit())
		{
			if (AShip* Ship = Cast<AShip>(GetInstigator()))
			{
				Ship->AddScore(Points);
			}
		}
		Destroy();
	}
}
