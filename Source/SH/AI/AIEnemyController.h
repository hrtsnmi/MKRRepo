// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */

class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAIPerceptionComponent;

UCLASS()
class SH_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()
	

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

public:
	AAIEnemyController();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Data", meta = (AllowPrivateAccess = true))
		TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Data", meta = (AllowPrivateAccess = true))
		TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

private:
	UPROPERTY() FTimerHandle EnemyTimer;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Data")
		float PatrolRadius{ 1000.f };

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Data")
		FName SearchTag {
		FName("Hiretsuna")
	};

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Data")
		FName HasLineOfSight {
		FName("HasLineOfSight")
	};

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Data")
		FName EnemyActor {
		FName("Target")
	};

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Data")
		FName DetectKnife {
		FName("DetectKnife")
	};

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Data")
		FName KnifeLocation {
		FName("TargetLocation")
	};
	

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Data")
		FName LostRecently {
		FName("LostRecently")
	};

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Data")
		float LineOfSightTime{ 4.0f };

public:
	UFUNCTION(BlueprintCallable, Category = "Enemy Data", meta = (AllowPrivateAccess = true))
		void PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION() void AfterTargetTeleports();

	UFUNCTION() void SetDetectKnife(const FVector& TargetLocation);
	UFUNCTION() void LostKnife();
};
