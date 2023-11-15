// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChasePlayer.h"
#include "../../Interfaces/WalkSpeedUpdateInterface.h"
#include "../AIEnemyController.h"

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type TaskResult = EBTNodeResult::Type::Failed;

	AAIEnemyController* AIController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	if (AIController && AIController->GetPawn())
	{
		APawn* Executer = AIController->GetPawn();
		if (!Executer)
		{
			return TaskResult;
		}

		bool bPawnHasInterface = Executer->GetClass()->ImplementsInterface(UWalkSpeedUpdateInterface::StaticClass());
		if (!bPawnHasInterface)
		{
			return TaskResult;
		}

		IWalkSpeedUpdateInterface::Execute_UpdateWalkSpeed(Executer, ChaseSpeed);

		TaskResult = EBTNodeResult::Type::Succeeded;
	}
	return TaskResult;
}
