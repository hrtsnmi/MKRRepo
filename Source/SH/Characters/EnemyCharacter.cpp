// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void AEnemyCharacter::UpdateWalkSpeed_Implementation(float NewWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;
}
