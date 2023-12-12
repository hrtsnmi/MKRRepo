// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TurretAttack.h"
#include "../TurretAIController.h"
#include "../Characters/SpiderCharacter.h"

EBTNodeResult::Type UBTT_TurretAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type TaskResult = EBTNodeResult::Type::Failed;

	ATurretAIController* AIControll = Cast<ATurretAIController>(OwnerComp.GetOwner());
	if (AIControll && AIControll->GetPawn())
	{
		ASpiderCharacter* Executer = Cast<ASpiderCharacter>(AIControll->GetPawn());

		if (Executer)
		{
			Executer->Fire();
			TaskResult = EBTNodeResult::Type::Succeeded;

		}
	}
	//EBTNodeResult::Type::Failed;
	//EBTNodeResult::Type::Succeeded;
	return TaskResult;
}
