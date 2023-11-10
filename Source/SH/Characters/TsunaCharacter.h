// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SHCharacter.h"
#include "Components/TimelineComponent.h"
#include "TsunaCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SH_API ATsunaCharacter : public ASHCharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ThrowAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* KnifeBackAction;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throw|Montage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ThrowMontage;

	UPROPERTY() FTimerHandle ThrowMontageTimer;
	UPROPERTY() FTimerHandle DeactivateNiagaraTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throw|Data", meta = (AllowPrivateAccess = "true"))
		float ThrowRate{ 1.f };

	bool bWasThrown{ false };

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
		class UNiagaraComponent* NS_LeakParticles;

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

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	ATsunaCharacter();

	virtual void BeginPlay() override;

protected:

	/** Called for movement input */
	void Throw(const FInputActionValue& Value);

	/** Called for movement input */
	void ShowParticlePath(const FInputActionValue& Value);

	/** Called for movement input */
	void SpawnKnifeBack(const FInputActionValue& Value);

};
