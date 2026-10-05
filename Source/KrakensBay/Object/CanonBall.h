// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CanonBall.generated.h"

UCLASS()
class KRAKENSBAY_API ACanonBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACanonBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CanonBall Properties")
	class UStaticMeshComponent* CanonBallMesh;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collision;
	
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* Movement;
	
	UPROPERTY(EditAnywhere, Category = "Water")
	float WaterHeight = 0.f;

	UPROPERTY(EditAnywhere, Category = "Water")
	UNiagaraSystem* SplashFX = nullptr;

	UFUNCTION()
	void OnBallOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
