// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionWarpingCharacter.h"
#include "Animation/AnimMontage.h"
#include "MotionWarping/Public/MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Interfaces/CanBeAttackInterface.h"

AMotionWarpingCharacter::AMotionWarpingCharacter()
{
	/*WarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(
		TEXT("Warping Component"));*/
}

void AMotionWarpingCharacter::Attack(AActor* Enemy)
{

	FTransform EnemytTransform = ICanBeAttackInterface::Execute_GetPreUnderAttack(Enemy);

	if (EnemytTransform.GetLocation() == FVector::ZeroVector)
	{
		return;
	}

	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance)
	{
		return;
	}
	if (!KillMontage)
	{
		return;
	}

	FMotionWarpingTarget TargetStart;
	TargetStart.Name = "KillStart";
	TargetStart.Location = EnemytTransform.GetLocation();
	TargetStart.Rotation = EnemytTransform.GetRotation().Rotator();

	/*FVector Direction = GetActorLocation() - tsuna.GetActorLocation();
	Direction.Normalize(0.0001f);
	float dotProdRes = FVector::DotProduct(tsuna.GetActorForwardVector(), Direction);

	if (dotProdRes <= 0.2f)
	{
		return false;
	}*/

	GetMotionWarpingComponent()->AddOrUpdateWarpTarget(TargetStart);

	GetWorldTimerManager().SetTimer(AfterVaultTH, this, &AMotionWarpingCharacter::AfterVault, AnimInstance->Montage_Play(KillMontage), false);
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	SetActorEnableCollision(false);
	bUseControllerRotationYaw = false;

	ICanBeAttackInterface::Execute_UnderAttack(Enemy);
}

void AMotionWarpingCharacter::Throw(const FInputActionValue& Value)
{
	/**/
	if (CharacterCrouchState == ECrouchStates::Crouch)
	{
		TSet<AActor*> Targets;
		GetOverlappingActors(Targets);
		if (Targets.Num() > 0)
		{
			AActor* Enemy = *(Targets.begin());

			if (Enemy->GetClass()->ImplementsInterface(UCanBeAttackInterface::StaticClass()))
			{
				Attack(Enemy);
				return;
			}
		}
	}
	Super::Throw(Value);
}

void AMotionWarpingCharacter::Interactive(const FInputActionValue& Value)
{
	bEButtonPressed = Value.Get<bool>();
}
	
void AMotionWarpingCharacter::AfterVault()
{
	GetWorldTimerManager().ClearTimer(AfterVaultTH);
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	SetActorEnableCollision(true);
	bUseControllerRotationYaw = true;
}
