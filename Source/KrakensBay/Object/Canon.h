// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CanonBall.h"
#include "GameFramework/Actor.h"
#include "Canon.generated.h"

UCLASS()
class KRAKENSBAY_API ACanon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACanon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Barrel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Muzzle;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACanonBall> CannonballClass;

	UPROPERTY(EditDefaultsOnly)
	float RecoilDistance = 25.f;

	UPROPERTY(EditDefaultsOnly)
	float RecoilReturnSpeed = 8.f;

	FVector BarrelRestLocation;
	UFUNCTION(BlueprintCallable, Category = "Canon Properties")
	void Shoot();
	
	UFUNCTION(BlueprintCallable, Category = "Canon Properties")
	FVector GetShotDirection() const;

	UPROPERTY(EditDefaultsOnly, Category = "Shot")
	FVector ShipSideAxis = FVector(0.f, 1.f, 0.f);
};
