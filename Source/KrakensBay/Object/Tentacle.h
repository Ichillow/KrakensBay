// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tentacle.generated.h"

UCLASS()
class KRAKENSBAY_API ATentacle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATentacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tentacle Properties")
	class USkeletalMeshComponent* TentacleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tentacle Properties")
	class UBoxComponent* TentacleBox;

	UPROPERTY(EditAnywhere, Category = "Tentacle Properties")
	float Speed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Tentacle Properties")
	float MapRadius = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Tentacle Properties")
	FVector MapCenter = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Tentacle Properties")
	class UAnimationAsset* SwimAnimation = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Tentacle Properties", meta = (ClampMin = "1"))
	int32 MinHealth = 1;

	UPROPERTY(EditAnywhere, Category = "Tentacle Properties", meta = (ClampMin = "1"))
	int32 MaxHealth = 5;

	UPROPERTY(VisibleInstanceOnly, Category = "Tentacle Properties")
	int32 CurrentHealth = 1;

	bool TakeHit();
	
	UPROPERTY(EditAnywhere, Category = "Tentacle Properties")
	float BlinkInterval = 0.07f;

	UPROPERTY(EditAnywhere, Category = "Tentacle Properties")
	int32 BlinkToggles = 6;

	FTimerHandle BlinkTimer;
	int32 BlinkCounter = 0;

	void StartBlink();
	void BlinkStep();
	
	UFUNCTION()
	void OnTentacleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, Category = "Tentacle Properties")
	int32 ScoreValue = 50;
	
};

