// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChasePlayer.h"
#include "../../Interfaces/WalkJogSwitcherInterface.h"
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

		bool bPawnHasInterface = Executer->GetClass()->ImplementsInterface(UWalkJogSwitcherInterface::StaticClass());
		if (!bPawnHasInterface)
		{
			return TaskResult;
		}

		IWalkJogSwitcherInterface::Execute_SetGaitData(Executer, ESpeedStates::Joging);

		TaskResult = EBTNodeResult::Type::Succeeded;
	}
	return TaskResult;
}
