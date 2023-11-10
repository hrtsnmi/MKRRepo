// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuantumtKnife.generated.h"

UCLASS()
class SH_API AQuantumtKnife : public AActor
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw|Data", meta = (AllowPrivateAccess = "true"))
		class UNiagaraComponent* NS_LeakParticlesProj;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Parts")
		class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Parts")
		class USkeletalMeshComponent* KnifeMesh;

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
	AQuantumtKnife();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UseTimeLineParticle(const FString& ParamaterName, float Value);

	void ActivateParticle(bool bActivate = false);

};