// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PursuitTarget.h"
#include "../AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTT_PursuitTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type TaskResult = EBTNodeResult::Type::Failed;

	AAIEnemyController* AIController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	if (!(AIController && AIController->GetPawn()))
	{
		return TaskResult;
	}


	ACharacter* Executer = Cast<ACharacter>(AIController->GetPawn());
	if (!Executer)
	{
		return TaskResult;
	}

	const FVector LastTargetLocation = AIController->GetBlackboardComponent()->GetValueAsVector(TargetLocation);
	float timeToLastLoc = (LastTargetLocation - AIController->GetPawn()->GetActorLocation()).Size() / Executer->GetCharacterMovement()->MaxWalkSpeed;
	FVector PosibleLocation = LastTargetLocation + AIController->GetBlackboardComponent()->GetValueAsVector(TargetVelocity) * timeToLastLoc;
	AIController->GetBlackboardComponent()->SetValueAsVector(TargetLocation, PosibleLocation);

	return EBTNodeResult::Type::Succeeded;
}
