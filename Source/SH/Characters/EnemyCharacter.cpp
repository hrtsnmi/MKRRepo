// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "../AI/AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AEnemyCharacter::AEnemyCharacter()
{
	StartPlace = CreateDefaultSubobject<USceneComponent>("StartPlace");
	StartPlace->SetupAttachment(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	BoxComp->SetupAttachment(StartPlace);

	HideKillWidget = CreateDefaultSubobject<UWidgetComponent>(
		TEXT("Widget Component"));
	HideKillWidget->SetupAttachment(RootComponent);
	HideKillWidget->SetVisibility(false);
}

void AEnemyCharacter::SetKnifeLoaction_Implementation(const FVector& Location)
{
	AAIEnemyController* AIContr = Cast<AAIEnemyController>(GetController());
	if (!AIContr) return;

	AIContr->GetBlackboardComponent()->SetValueAsBool(FName("Knife"), true);
	//AIContr->GetBlackboardComponent()->SetValueAsBool(FName("HasLineOfSight"), false);
	AIContr->GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), Location);
}

bool AEnemyCharacter::UnderAttack_Implementation()
{
	if (this->HideKillWidget)
	{
		HideKillWidget->SetVisibility(false);
	}

	if (!AttackedByMontage)
	{
		return false;
	}

	GetWorldTimerManager().SetTimer(AttackedByTH, this, &AEnemyCharacter::AfterAttackedBy, 0.8 * PlayAnimMontage(AttackedByMontage), false);
	return true;
}

FTransform AEnemyCharacter::GetPreUnderAttack_Implementation()
{
	return StartPlace->GetComponentTransform();
}


void AEnemyCharacter::AfterAttackedBy()
{
	GetWorldTimerManager().ClearTimer(AttackedByTH);
	GetMesh()->SetSimulatePhysics(true);
	this->Destroy();
}

void AEnemyCharacter::SetPatrolData_Implementation(EPatrolStates State)
{
	PatrolState = State;
}

//void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	
//		Super::SetupPlayerInputComponent(PlayerInputComponent);
//		// Set up action bindings
//		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
//		{
//
//			//E
//			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AEnemyCharacter::ResetValue);
//			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AEnemyCharacter::SetValue);
//		}
//}

EPatrolStates AEnemyCharacter::RecivePatrolData_Implementation()
{
	return PatrolState;
}


FVector AEnemyCharacter::GoForwardBackwardPath_Implementation()
{
	// TODO: insert return statement here
	if (PatrolPath.Num() > 0)
	{
		if (bForwardFollowing && i >= PatrolPath.Num())
		{
			bForwardFollowing = false;
			i = PatrolPath.Num() - 1;
		}

		if (!bForwardFollowing && i < 0)
		{
			bForwardFollowing = true;
			i = 0;
		}

		if (bForwardFollowing)
		{
			return PatrolPath[i++]->GetActorLocation();
		}
		else
		{
			return PatrolPath[i--]->GetActorLocation();
		}
	}
	else
	{
		return GetActorLocation();
	}
}

FVector AEnemyCharacter::GoCyclePath_Implementation()
{
	if (PatrolPath.Num() > 0)
	{
		const FVector& result = PatrolPath[i]->GetActorLocation();
		i++;
		i %= PatrolPath.Num();
		return result;
	}
	else
	{
		return GetActorLocation();
	}
}
