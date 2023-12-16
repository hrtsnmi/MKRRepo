// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraCharacter.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Components/SplineMeshComponent.h"
#include "ThrowCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddSplineMeshAtIndexSignature, int32, index);
DECLARE_MULTICAST_DELEGATE(FCharacterDisappearSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetVisabilitySignature, bool, bIsVisible);

void ClearSpline(TArray<USplineMeshComponent*>& SplineMeshes, class USplineComponent* KnifeSpline);

/**
 * 
 */
UCLASS()
class SH_API AThrowCharacter : public ANiagaraCharacter
{
	GENERATED_BODY()
	
public:
	AThrowCharacter();

private:

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throw|Montage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ThrowMontage;
	UPROPERTY() FTimerHandle ThrowMontageTimer;
	UPROPERTY() FTimerHandle DeactivateNiagaraTimer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throw|Data", meta = (AllowPrivateAccess = "true"))
		float ThrowRate{ 1.f };
	bool bWasThrown{ false };
	UPROPERTY() FVector ThownLocation { FVector(84.92f, -15.0f, 41.48f) };
	UPROPERTY() FTransform ThownTransform;

	UPROPERTY() FPredictProjectilePathParams PredictParams;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throw|Data", meta = (AllowPrivateAccess = "true"))
		USplineComponent* KnifeSpline;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throw|Data", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* EndSpline;
	UPROPERTY(BlueprintAssignable)
		FAddSplineMeshAtIndexSignature OnAddSplineMeshAtIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Throw|Data", meta = (AllowPrivateAccess = "true"))
		TArray<USplineMeshComponent*> SplineMeshes;

	//Spawn Data
private:
	/*The timeline that will make the actor float*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw|Data", meta = (AllowPrivateAccess = "true"))
		UTimelineComponent* SpawnTimeline;
	FOnTimelineFloat ProgressFunction;
	FOnTimelineEvent SpawnTimelineFinishedEvent;
	UPROPERTY(VisibleAnywhere) class AQuantumtKnife* ThownProjectile{ nullptr };
	/*The function that will handle every tick of the float curve*/
	UFUNCTION()
		void HandleSpawnProgress(float Value);
	UFUNCTION()
		void SpawnTimelineFinishedFunction();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw|Data", meta = (AllowPrivateAccess = "true"))
		FName ParamaterName = "Animation";

public:
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* SpawnCurveFloat;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knife", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* KnifeLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knife", meta = (AllowPrivateAccess = "true"))
		class URectLightComponent* RectLightComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knife", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Knife;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knife", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AQuantumtKnife> KnifeProj;

	//for camera
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knife|Offset", meta = (AllowPrivateAccess = "true"))
		FVector AIMData {
		FVector(0.f, 100.f, 100.f)
	};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knife|Offset", meta = (AllowPrivateAccess = "true"))
		float AimOfset{ 200.f };

	FVector PrevCameraData{ };
	float PrevAimOfset{ };
	bool bIsAim{ false };

protected:
	virtual void BeginPlay() override;

protected:
	/** Called for movement input */
	virtual void Throw(const FInputActionValue& Value) override;
	virtual void RemoveParticlePath(const FInputActionValue& Value) override;
	/** Called for movement input */
	virtual void ShowParticlePath(const FInputActionValue& Value) override;
	/** Called for movement input */
	virtual void SpawnKnifeBack(const FInputActionValue& Value) override;

	
	UPROPERTY(BlueprintAssignable)
		FSetVisabilitySignature OnSetVisabilityDelegate;

	UPROPERTY()
		FTimerHandle Location2SecAgoTimer;
	UPROPERTY(BlueprintReadOnly) TMap<FVector, FTimerHandle> LocationSavedFor2Sec;
	UPROPERTY(EditDefaultsOnly) bool bUseTimers{ false };
	UFUNCTION(BlueprintCallable)void SetUpTimers(bool bIsActive = false);

public:
	//for AI
	FCharacterDisappearSignature OnCharacterDisappearDelegate;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Throw|Speed")
		float StartSpeed{ 2000.f };
};
