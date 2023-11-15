// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_WriteTargetVelocyty.h"
#include "../AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/ToTsunaHierarchy/ThrowCharacter.h"


UBTS_WriteTargetVelocyty::UBTS_WriteTargetVelocyty(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "RotateToTarget";
	BlackboardKey.AddObjectFilter(this, FName(TEXT("Target")), AActor::StaticClass());
}


void UBTS_WriteTargetVelocyty::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIEnemyController* AIControll = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	if (!AIControll || !AIControll->GetPawn())
	{
		return;
	}

	AThrowCharacter* Target = Cast <AThrowCharacter>(AIControll->GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target)
	{
		AIControll->GetBlackboardComponent()->SetValueAsVector(TargetVelocityKey, Target->GetVelocity());
		AIControll->GetBlackboardComponent()->SetValueAsVector(TargetLocationKey, Target->GetActorLocation());

		if (!Target->OnCharacterDisappearDelegate.IsBoundToObject(this))
		{
			Target->OnCharacterDisappearDelegate.AddUObject(AIControll, &AAIEnemyController::AfterTargetTeleports);
			
				//OnScoreChangedDelegate.AddUObject(this, &ThisClass::OnScoreChanged);
		}
	}
}
