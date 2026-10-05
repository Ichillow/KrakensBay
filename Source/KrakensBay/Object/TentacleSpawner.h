// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TentacleSpawner.generated.h"

UCLASS()
class KRAKENSBAY_API ATentacleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATentacleSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<class ATentacle> TentacleClass;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "0.1"))
	float MinSpawnDelay = 2.f;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "0.1"))
	float MaxSpawnDelay = 6.f;

	void ScheduleNextSpawn();

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnRadius = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector Center = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnHeight = 0.f;

	FTimerHandle SpawnTimer;

	void SpawnTentacle();
	
	
};
