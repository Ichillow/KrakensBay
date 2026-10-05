// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Canon.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Ship.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLivesChanged, int32, CurrentLives, int32, MaxLives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);

UCLASS()
class KRAKENSBAY_API AShip : public APawn
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	AShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void TurnReleased(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* ShipMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = "Ship")
	float MaxTurnSpeed = 90.f;

	UPROPERTY(EditAnywhere, Category = "Ship")
	float TurnResponsiveness = 2.f;
	
private:
	float TurnInput = 0.f;
	float CurrentTurnRate = 0.f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Properties")
	class UStaticMeshComponent* ShipMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Properties")
	class UBoxComponent* CollisionBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Properties")
	class UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* Movement;
	
	void Look(const FInputActionValue& Value);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ACanon*> Cannons;
	
	UPROPERTY(EditDefaultsOnly, Category = "Cannons")
	TSubclassOf<ACanon> CannonClass;

	UPROPERTY(EditDefaultsOnly, Category = "Cannons")
	TArray<FName> CannonSockets;
	
	UFUNCTION(BlueprintCallable)
	void FireCannons();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Limit")
    FVector Center = FVector::ZeroVector;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Limit")
    float Radius = 5000.f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lives")
	int32 MaxLives = 3;

	UPROPERTY(BlueprintReadOnly, Category = "Lives")
	int32 CurrentLives = 3;

	UPROPERTY(EditAnywhere, Category = "Lives")
	float InvincibilityTime = 1.5f;

	UPROPERTY(BlueprintAssignable, Category = "Lives")
	FOnLivesChanged OnLivesChanged;

	UPROPERTY(EditAnywhere, Category = "Lives")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	void LoseLife();

	bool bInvincible = false;
	FTimerHandle InvincibleTimer;
	FTimerHandle RestartTimer;
	void EndInvincibility();
	void RestartLevel();
	
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 Score = 0;

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreChanged OnScoreChanged;

	void AddScore(int32 Points);
	
	UPROPERTY(EditAnywhere, Category = "Lives")
    FName MenuLevelName = "Menu_Map";
};
