// Fill out your copyright notice in the Description page of Project Settings.


#include "Canon.h"

// Sets default values
ACanon::ACanon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Barrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel"));
	Barrel->SetupAttachment(Root);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(Barrel);
}

// Called when the game starts or when spawned
void ACanon::BeginPlay()
{
	Super::BeginPlay();
	
	BarrelRestLocation = Barrel->GetRelativeLocation();
	
}

// Called every frame
void ACanon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const FVector NewLoc = FMath::VInterpTo(
		Barrel->GetRelativeLocation(), BarrelRestLocation, DeltaTime, RecoilReturnSpeed);
	Barrel->SetRelativeLocation(NewLoc);

}

void ACanon::Shoot()
{	
	if (!CannonballClass) return;
		
	const FVector Dir = GetShotDirection();
	
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = GetInstigator();

	ACanonBall* Ball = GetWorld()->SpawnActor<ACanonBall>(CannonballClass,
		Muzzle->GetComponentLocation(), Muzzle->GetComponentRotation(), Params);

	if (Ball && GetAttachParentActor())
	{
		Ball->Collision->IgnoreActorWhenMoving(GetAttachParentActor(), true);
	}

	Barrel->SetRelativeLocation(BarrelRestLocation - FVector(0.f, RecoilDistance, 0.f));
}

FVector ACanon::GetShotDirection() const
{
	AActor* ShipActor = GetAttachParentActor();
	if (!ShipActor) return GetActorForwardVector();

	const FVector SideAxis = ShipActor->GetActorTransform()
		.TransformVectorNoScale(ShipSideAxis.GetSafeNormal());

	const FVector ToCannon = GetActorLocation() - ShipActor->GetActorLocation();
	const float Side = FVector::DotProduct(ToCannon, SideAxis) >= 0.f ? 1.f : -1.f;

	return SideAxis * Side;
}