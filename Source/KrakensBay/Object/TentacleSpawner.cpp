// Fill out your copyright notice in the Description page of Project Settings.


#include "TentacleSpawner.h"
#include "Tentacle.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATentacleSpawner::ATentacleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATentacleSpawner::BeginPlay()
{
	Super::BeginPlay();
	ScheduleNextSpawn();
	
	
}

// Called every frame
void ATentacleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATentacleSpawner::SpawnTentacle()
{
	ScheduleNextSpawn();
	
	if (!TentacleClass) return;
	

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;

	const float Angle = FMath::FRandRange(0.f, 2.f * PI);
	FVector SpawnLoc = Center + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f) * SpawnRadius;
	SpawnLoc.Z = SpawnHeight;


	FVector Dir = Player->GetActorLocation() - SpawnLoc;
	Dir.Z = 0.f;
	Dir = Dir.GetSafeNormal();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ATentacle>(TentacleClass, SpawnLoc, Dir.Rotation(), Params);
}

void ATentacleSpawner::ScheduleNextSpawn()
{
	const float Delay = FMath::FRandRange(MinSpawnDelay, FMath::Max(MinSpawnDelay, MaxSpawnDelay));
	GetWorldTimerManager().SetTimer(
		SpawnTimer, this, &ATentacleSpawner::SpawnTentacle, Delay, false); // false = une seule fois
}