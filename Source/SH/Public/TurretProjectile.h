// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretProjectile.generated.h"

UCLASS()
class SH_API ATurretProjectile : public AActor
{
	GENERATED_BODY()

private:
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Parts")
		class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Parts")
		class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditAnywhere, Category = "Combat|Effects")
		class UParticleSystemComponent* TrailParticle;

	UPROPERTY(EditAnywhere, Category = "Combat|Effects")
		class UParticleSystem* HitPractices;

	UPROPERTY(EditAnywhere, Category = "Combat|Sound")
		class USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat|Sound")
		USoundBase* HitSound;

public:	
	// Sets default values for this actor's properties
	ATurretProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	float& AddOwnerSpeed();
	void SetSize(float radius);
};
