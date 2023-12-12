// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Characters/TsunaCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATurretProjectile::ATurretProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	
	RootComponent = SphereComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(SphereComponent);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	

	if (LaunchSound)
		UGameplayStatics::PlaySoundAtLocation(this,
			LaunchSound,
			GetActorLocation());
}

// Called when the game starts or when spawned
void ATurretProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurretProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


float& ATurretProjectile::AddOwnerSpeed()
{
	return ProjectileMovementComp->InitialSpeed;
}


void ATurretProjectile::SetSize(float radius)
{
	SphereComponent->SetSphereRadius(radius);
}


void ATurretProjectile::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (this == OtherActor)
		return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Red,
			FString::Printf(TEXT("Projectile hits %s"), *OtherActor->GetName()));
	}

	if (Cast<ATsunaCharacter>(OtherActor))
	{
		if (HitPractices)
			UGameplayStatics::SpawnEmitterAtLocation(this,
				HitPractices,
				GetActorLocation(),
				GetActorRotation());

		if (HitSound)
			UGameplayStatics::PlaySoundAtLocation(this,
				HitSound,
				GetActorLocation());
	}

	Destroy();
}