// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SHCharacter.h"
#include "SpiderCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SH_API ASpiderCharacter : public ASHCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	ASpiderCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//for Debug Draw
	UFUNCTION()
		void DrawDebugElements() const;

	void SetNewAnimPitch(bool ToTarget = false);

	void SetTergetTall(AActor* Target);
	//lines
	UPROPERTY()
		TArray<FVector>LinesStartEnds;
	//spheres
	UPROPERTY()
		FVector PointToShootCenter;

	float NotRangeRadius1{};
	float NotRangeRadius2{};
	float RangeRadius{};

protected:
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Base;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Combat")
		TSubclassOf<class ATurretProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Speed", meta = (AllowPrivateAccess = "true"))
		float ProjectileSpeed{ 3900.f };

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Params", meta = (AllowPrivateAccess = "true"))
		float ProjectileScale{ 0.3f };

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Params", meta = (AllowPrivateAccess = "true"))
		float ProjectileCapsuleRadius{ 55.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimPitch", meta = (AllowPrivateAccess = "true"))
		float AnimPitch{ };

	UPROPERTY(VisibleAnywhere, Category = "Target", meta = (AllowPrivateAccess = "true"))
		float TargetHalfTall;

	constexpr static float boardAngle{ 45.f };
	constexpr static float boardAngleRad{ boardAngle * PI / 180.f };

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly) float searchRadius{ 800.0f };

	UFUNCTION(BlueprintCallable) void Fire();

	//func to check if in Range
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ObjClass"))
		UObject* CheckIfInRange();

	UFUNCTION()
		void PrepareFutureHitPoint(AActor* Target, FVector& OutToShoot);

	//func for Rotate Turret
	void RotateTurret();

	UFUNCTION() float GetAnimPitch() const { return AnimPitch; }
};


float CalcQuadraticEquation(float a, float b, float c);
float CalcQuadraticEquation(float a, float b, float c, float& x1, float& x2);