// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomPatrol.h"
#include "../AIEnemyController.h"
#include "../../Interfaces/PatrolStateInterface.h"
#include "../../Interfaces/WalkSpeedUpdateInterface.h"
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

	bool WalkSpeedUpdateInterface = Executer->GetClass()->ImplementsInterface(UWalkSpeedUpdateInterface::StaticClass());
	if (!WalkSpeedUpdateInterface)
	{
		return TaskResult;
	}

	IWalkSpeedUpdateInterface::Execute_UpdateWalkSpeed(Executer, PatrolSpeed);

	FVector NewLocation;
	bool bPawnHasFollowPatrolPathInterface =
		Executer->GetClass()->ImplementsInterface(UFollowPatrolPathInterface::StaticClass());
	if (Executer->GetClass()->ImplementsInterface(UPatrolStateInterface::StaticClass()))
	{
		FNavLocation NewLoc;
		bool bFindNewPoint = false;
		EPatrolStates UsingPatrolState = IPatrolStateInterface::Execute_RecivePatrolData(Executer);

		switch (UsingPatrolState)
		{
		case EPatrolStates::Random:
			//GetRandomPoint
			
			bFindNewPoint = UNavigationSystemV1::GetNavigationSystem(&OwnerComp)->
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
			break;
		case EPatrolStates::Cycle:
			if (bPawnHasFollowPatrolPathInterface)
			{
				NewLocation = IFollowPatrolPathInterface::Execute_GoCyclePath(Executer);

				/*if (GEngine)  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
					FString::Printf(TEXT("New Location %s"), *NewLocation.ToString()));*/

				AIController->GetBlackboardComponent()->SetValueAsVector(NewLocationKey, NewLocation);

				TaskResult = EBTNodeResult::Type::Succeeded;
			}
			break;
		case EPatrolStates::FwdBwd:
			if (bPawnHasFollowPatrolPathInterface)
			{
				NewLocation = IFollowPatrolPathInterface::Execute_GoForwardBackwardPath(Executer);

				/*if (GEngine)  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
					FString::Printf(TEXT("New Location %s"), *NewLocation.ToString()));*/

				AIController->GetBlackboardComponent()->SetValueAsVector(NewLocationKey, NewLocation);

				TaskResult = EBTNodeResult::Type::Succeeded;
			}
			break;
		default:
			return TaskResult;
		}
	}

	return TaskResult;
}
