// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PursuitTarget.generated.h"

/**
 * 
 */
UCLASS()
class SH_API UBTT_PursuitTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY(EditInstanceOnly)
		FName TargetVelocity {
		FName("TargetVelocity")
	};

	UPROPERTY(EditInstanceOnly)
		FName TargetLocation {
		FName("TargetLocation")
	};


};
