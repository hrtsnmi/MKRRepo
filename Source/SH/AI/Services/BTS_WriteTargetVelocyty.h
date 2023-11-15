// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_WriteTargetVelocyty.generated.h"

/**
 * 
 */
UCLASS()
class SH_API UBTS_WriteTargetVelocyty : public UBTService_BlackboardBase
{
	GENERATED_BODY()

private:
	UBTS_WriteTargetVelocyty(const FObjectInitializer& ObjectInitializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

	UPROPERTY(EditInstanceOnly)
		FName TargetVelocityKey { "TargetVelocity" };

	UPROPERTY(EditInstanceOnly)
		FName TargetLocationKey {
		"TargetLocation"
	};
	
};
