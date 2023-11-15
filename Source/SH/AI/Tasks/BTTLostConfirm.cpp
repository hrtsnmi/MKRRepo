// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTLostConfirm.h"
#include "../AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTLostConfirm::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIEnemyController* AIController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	if (!(AIController && AIController->GetPawn()))
	{
		return EBTNodeResult::Type::Failed;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(LostRecently, false);

	return EBTNodeResult::Type::Succeeded;
}
