// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"

void AAIEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(BehaviorTree.Get()))
	{
		RunBehaviorTree(BehaviorTree.Get());
	}
}

void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIEnemyController::PerceptionUpdated);
}

AAIEnemyController::AAIEnemyController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(
		TEXT("AIPerception")
	);

}

void AAIEnemyController::PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		return;
	}

	bool ActorHasTag = Actor->ActorHasTag(SearchTag) && Stimulus.WasSuccessfullySensed();
	if (ActorHasTag)
	{
		GetWorldTimerManager().ClearTimer(EnemyTimer);

		UBlackboardComponent* BB = GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsBool(HasLineOfSight, true);
			BB->SetValueAsObject(EnemyActor, Actor);
			BB->SetValueAsBool(LostRecently, false);
		}
	}
	else
	{
		GetWorldTimerManager().SetTimer(EnemyTimer, FTimerDelegate::CreateLambda([this, Actor]
			{
				UBlackboardComponent* BB = GetBlackboardComponent();
				if (BB)
				{
					BB->SetValueAsBool(HasLineOfSight, false);
					BB->SetValueAsObject(EnemyActor, nullptr);
					BB->SetValueAsBool(LostRecently, true);
				}
			}),
			LineOfSightTime, false);
	}
}

void AAIEnemyController::AfterTargetTeleports()
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (BB)
	{
		BB->SetValueAsBool(HasLineOfSight, false);
		BB->SetValueAsObject(EnemyActor, nullptr);
		BB->SetValueAsBool(LostRecently, true);
	}
}

void AAIEnemyController::SetDetectKnife(const FVector& TargetLocation)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (BB)
	{
		BB->SetValueAsBool(DetectKnife, true);
		BB->SetValueAsVector(KnifeLocation, TargetLocation);
	}
}

void AAIEnemyController::LostKnife()
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (BB)
	{
		BB->SetValueAsBool(DetectKnife, false);
	}
}
