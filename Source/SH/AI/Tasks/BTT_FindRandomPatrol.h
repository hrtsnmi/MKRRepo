// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FindRandomPatrol.generated.h"

/**
 * 
 */
UCLASS()
class SH_API UBTT_FindRandomPatrol : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditInstanceOnly)
		float PatrolSpeed{ 200.f };

	UPROPERTY(EditInstanceOnly)
		FName NewLocationKey {
		FName("PatrolLocation")
	};

};
