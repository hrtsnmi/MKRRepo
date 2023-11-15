// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PatrolMode.h"
#include "../AIEnemyController.h"
#include "../../Interfaces/FollowPatrolPathInterface.h"
#include "../../Interfaces/WalkJogSwitcherInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_PatrolMode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (Executer->GetClass()->ImplementsInterface(UFollowPatrolPathInterface::StaticClass()))
	{
		FVector NewLocation;
		bool bFindNewPoint = false;
		EPatrolStates UsingPatrolState = IFollowPatrolPathInterface::Execute_RecivePatrolData(Executer);

		switch (UsingPatrolState)
		{
		case EPatrolStates::Cycle:
				NewLocation = IFollowPatrolPathInterface::Execute_GoCyclePath(Executer);

				/*if (GEngine)  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
					FString::Printf(TEXT("New Location %s"), *NewLocation.ToString()));*/

				AIController->GetBlackboardComponent()->SetValueAsVector(NewLocationKey, NewLocation);

				TaskResult = EBTNodeResult::Type::Succeeded;
			break;
		case EPatrolStates::FwdBwd:
				NewLocation = IFollowPatrolPathInterface::Execute_GoForwardBackwardPath(Executer);

				/*if (GEngine)  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
					FString::Printf(TEXT("New Location %s"), *NewLocation.ToString()));*/

				AIController->GetBlackboardComponent()->SetValueAsVector(NewLocationKey, NewLocation);

				TaskResult = EBTNodeResult::Type::Succeeded;
			break;
		default:
			return TaskResult;
		}
	}

	return TaskResult;
}
