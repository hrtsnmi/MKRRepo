// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "../AI/AIEnemyController.h"
#include "../QuantumtKnife.h"

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

void AEnemyCharacter::SetKnifeLocation_Implementation(AQuantumtKnife* Target)
{
	if (AAIEnemyController* AIController = Cast<AAIEnemyController>(GetController()))
	{
		AIController->SetDetectKnife(Target->GetActorLocation());
			
		if (!Target->OnActorDestroyDelegate.IsBoundToObject(AIController))
		{
			Target->OnActorDestroyDelegate.AddUObject(AIController, &AAIEnemyController::LostKnife);
		}
	}
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
