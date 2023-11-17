// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToTsunaHierarchy/NiagaraCharacter.h"
#include "../Interfaces/FollowPatrolPathInterface.h"
#include "../Interfaces/CanBeAttackInterface.h"
#include "../Interfaces/FollowPatrolPathInterface.h"
#include "Engine/TargetPoint.h"
#include "ToTsunaHierarchy/LookAtKnifeInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */

UCLASS()
class SH_API AEnemyCharacter : public ANiagaraCharacter,
	public IFollowPatrolPathInterface,
	public ICanBeAttackInterface,
	public ILookAtKnifeInterface
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	void SetKnifeLoaction_Implementation(const FVector& Location);

protected:
	UPROPERTY(EditAnywhere, Category = "ToUnderAttack", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* StartPlace;
	UPROPERTY(EditAnywhere, Category = "ToUnderAttack", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Warping Component")
		UAnimMontage* AttackedByMontage;

	UPROPERTY(EditAnywhere, Category = "Widget Component")
		class UWidgetComponent* HideKillWidget;

	FTimerHandle AttackedByTH;

	UPROPERTY(EditInstanceOnly, Category = "AI|Path", meta = (AllowPrivateAccess = "true"))
		TArray<class ATargetPoint*> PatrolPath;
	int i{};
	bool bForwardFollowing{ true };

protected:
	void AfterAttackedBy();

	//AI
protected:
	void SetPatrolData_Implementation(EPatrolStates State);
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	EPatrolStates RecivePatrolData_Implementation();

	FVector GoForwardBackwardPath_Implementation();
	FVector GoCyclePath_Implementation();

protected:
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
		EPatrolStates PatrolState {EPatrolStates::Random};

public:
	bool UnderAttack_Implementation();
	FTransform GetPreUnderAttack_Implementation();


};
