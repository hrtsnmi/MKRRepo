// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomPatrol.h"
#include "../AIEnemyController.h"
#include "../../Interfaces/WalkJogSwitcherInterface.h"
#include "../../Interfaces/FollowPatrolPathInterface.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type TaskResult = EBTNodeResult::Type::Failed;

	AAIEnemyController* AIController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	if (!(AIController && AIController->GetPawn()))
	{
		return TaskResult;
	}


	APawn* Executer = AIController->GetPawn();
	if (!Executer)
	{
		return TaskResult;
	}

	bool WalkSpeedUpdateInterface = Executer->GetClass()->ImplementsInterface(UWalkJogSwitcherInterface::StaticClass());
	if (!WalkSpeedUpdateInterface)
	{
		return TaskResult;
	}

	IWalkJogSwitcherInterface::Execute_SetGaitData(Executer, ESpeedStates::Walking);

	FVector NewLocation;
	bool bPawnHasFollowPatrolPathInterface =
		Executer->GetClass()->ImplementsInterface(UFollowPatrolPathInterface::StaticClass());
	
	FNavLocation NewLoc;
	bool bFindNewPoint = UNavigationSystemV1::GetNavigationSystem(&OwnerComp)->
		GetRandomReachablePointInRadius(AIController->GetNavAgentLocation(),
			AIController->PatrolRadius, NewLoc);

	if (bFindNewPoint)
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(NewLocationKey, NewLoc);
	}
	else
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(NewLocationKey, Executer->GetActorLocation());
	}

	TaskResult = EBTNodeResult::Type::Succeeded;


	return TaskResult;
}
