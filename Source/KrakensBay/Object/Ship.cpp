// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	CollisionBox->SetGenerateOverlapEvents(true);
	RootComponent = CollisionBox;

	// Create StaticMeshComponent and Attach to BoxComponent
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	ShipMesh->SetupAttachment(CollisionBox);	
	

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CollisionBox);
	SpringArm->TargetArmLength = 1500.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 500.f));
	
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 1200.f;
	Movement->Acceleration = 800.f;
	Movement->Deceleration = 400.f;
	
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentLives = MaxLives;
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ShipMappingContext, 0);
		}
		
		PC->SetControlRotation(FRotator(-45.f, GetActorRotation().Yaw, 0.f)); 
		if (PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->ViewPitchMin = -80.f;   
			PC->PlayerCameraManager->ViewPitchMax = -5.f; 
		}
		
		if (HUDWidgetClass)
		{
			if (UUserWidget* HUD = CreateWidget<UUserWidget>(PC, HUDWidgetClass))
			{
				HUD->AddToViewport();
			}
		}
	}
	
	TArray<UChildActorComponent*> ChildComps;
	GetComponents<UChildActorComponent>(ChildComps);
	
	if (!CannonClass) return;

	for (const FName& Socket : CannonSockets)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.Instigator = this;

		ACanon* Cannon = GetWorld()->SpawnActor<ACanon>(
			CannonClass, ShipMesh->GetSocketTransform(Socket), Params);

		if (Cannon)
		{
			Cannon->AttachToComponent(ShipMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
			Cannons.Add(Cannon);
		}
	}
	
}

void AShip::Move(const FInputActionValue& Value)
{
	AddMovementInput(GetActorForwardVector(), Value.Get<float>());
}

void AShip::Turn(const FInputActionValue& Value)
{
	TurnInput = Value.Get<float>();
}

void AShip::TurnReleased(const FInputActionValue& Value)
{
	TurnInput = 0.0f;
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentTurnRate = FMath::FInterpTo(CurrentTurnRate, TurnInput * MaxTurnSpeed, DeltaTime, TurnResponsiveness);
	AddActorWorldRotation(FRotator(0.f, CurrentTurnRate * DeltaTime, 0.f));

	FVector Offset = GetActorLocation() - Center;
	Offset.Z = 0;
	if (Offset.Size() > Radius)
	{
		FVector Clamped = Center + Offset.GetSafeNormal() * Radius;
		SetActorLocation(FVector(Clamped.X, Clamped.Y, GetActorLocation().Z));
	}
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShip::Move);
		EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AShip::Turn);
		EIC->BindAction(TurnAction, ETriggerEvent::Completed, this, &AShip::TurnReleased);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShip::Look);
		EIC->BindAction(ShootAction, ETriggerEvent::Started, this, &AShip::FireCannons);
	}
}

void AShip::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void AShip::FireCannons()
{
	FVector CamDir = GetActorForwardVector();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (PC->PlayerCameraManager)
		{
			const FRotator CamRot = PC->PlayerCameraManager->GetCameraRotation();
			CamDir = CamRot.Vector();
			CamDir.Z = 0.f;

			// Caméra quasi verticale (vue du dessus) : on prend le "haut" de l'écran
			if (CamDir.IsNearlyZero(0.01f))
			{
				CamDir = FRotationMatrix(CamRot).GetUnitAxis(EAxis::Z);
				CamDir.Z = 0.f;
			}
		}
	}
	CamDir = CamDir.GetSafeNormal();

	for (ACanon* Cannon : Cannons)
	{
		if (!Cannon) continue;

		if (FVector::DotProduct(Cannon->GetShotDirection(), CamDir) > 0.3f)
		{
			Cannon->Shoot();
		}
	}
}

void AShip::LoseLife()
{
	if (bInvincible || CurrentLives <= 0) return;

	CurrentLives--;
	OnLivesChanged.Broadcast(CurrentLives, MaxLives);

	if (CurrentLives <= 0)
	{
		// Game over : on bloque les contrôles puis on relance le niveau
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			DisableInput(PC);
		}
		GetWorldTimerManager().SetTimer(RestartTimer, this, &AShip::RestartLevel, 1.0f, false);
		return;
	}

	bInvincible = true;
	GetWorldTimerManager().SetTimer(InvincibleTimer, this, &AShip::EndInvincibility, InvincibilityTime, false);
}

void AShip::EndInvincibility()
{
	bInvincible = false;
}

void AShip::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, MenuLevelName);
}

void AShip::AddScore(int32 Points)
{
	Score += Points;
	OnScoreChanged.Broadcast(Score);
}