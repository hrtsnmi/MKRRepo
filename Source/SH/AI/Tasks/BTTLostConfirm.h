// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTLostConfirm.generated.h"

/**
 * 
 */
UCLASS()
class SH_API UBTTLostConfirm : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:


	UPROPERTY(EditInstanceOnly)
		FName LostRecently {
		FName("LostRecently")
	};
};
