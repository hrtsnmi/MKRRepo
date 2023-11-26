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

	/*FTransform EnemytTransform = ICanBeAttackInterface::Execute_GetPreUnderAttack(Enemy);

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
	TargetStart.Rotation = EnemytTransform.GetRotation().Rotator();*/

	/*FVector Direction = GetActorLocation() - tsuna.GetActorLocation();
	Direction.Normalize(0.0001f);
	float dotProdRes = FVector::DotProduct(tsuna.GetActorForwardVector(), Direction);

	if (dotProdRes <= 0.2f)
	{
		return false;
	}*/

	/*WarpingComponent->AddOrUpdateWarpTarget(TargetStart);

	GetWorldTimerManager().SetTimer(AfterVaultTH, this, &AMotionWarpingCharacter::AfterVault, AnimInstance->Montage_Play(KillMontage), false);
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	SetActorEnableCollision(false);
	bUseControllerRotationYaw = false;

	ICanBeAttackInterface::Execute_UnderAttack(Enemy);*/
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




bool AMotionWarpingCharacter::Vault(FVector& VStart, FVector& VMid, FVector& VEnd)
{
	//for (size_t x = 0; x < 3; x++)
	//{
	//	FHitResult HorizHitRes, VertHitRes;

	//	float dictance = 200.f;

	//	FVector StartHoriz = GetActorLocation();
	//	StartHoriz.Z += x * 30;
	//	FVector EndHoriz = GetActorForwardVector() * dictance + StartHoriz;

	//	FCollisionShape Sphere = FCollisionShape::MakeSphere(1.f);
	//	bool HasHit = GetWorld()->SweepSingleByChannel(
	//		HorizHitRes,
	//		StartHoriz, EndHoriz,
	//		FQuat::Identity,
	//		ECollisionChannel::ECC_Visibility,
	//		Sphere);
	//	DrawDebugSphere(GetWorld(), EndHoriz, 3.f, 3.f, FColor::Red, true, 5);
	//	DrawDebugLine(GetWorld(), StartHoriz, EndHoriz, FColor::Red, true, 1, 1.f);

	//	if (HasHit)
	//	{
	//		FVector StartVert, EndVert;

	//		StartVert = HorizHitRes.Location;
	//		EndVert = StartVert;
	//		StartVert.Z += 50.f;

	//		for (size_t y = 0; y < 5; y++)
	//		{
	//			bool VertHit = GetWorld()->SweepSingleByChannel(
	//				VertHitRes,
	//				StartVert, EndVert,
	//				FQuat::Identity,
	//				ECollisionChannel::ECC_Visibility,
	//				Sphere);

	//			if (y == 0)
	//			{
	//				VStart = VertHitRes.Location;
	//			}
	//			if (VertHit)
	//			{
	//				VMid = VertHitRes.Location;
	//			}
	//			//			
	//			if (!VertHit)
	//			{
	//				//DrawDebugSphere(GetWorld(), VMid, 10.f, 5, FColor::Red, true);
	//				if (
	//					GetWorld()->SweepSingleByChannel(
	//						HorizHitRes,
	//						VertHitRes.TraceStart + GetActorForwardVector() * 50.f,
	//						FVector(VertHitRes.TraceStart.X, VertHitRes.TraceStart.Y, VertHitRes.TraceStart.Z - 1000.f),
	//						FQuat::Identity,
	//						ECollisionChannel::ECC_Visibility,
	//						Sphere)
	//					)
	//				{
	//					VEnd = HorizHitRes.Location;
	//					DrawDebugSphere(GetWorld(), VStart, 10.f, 5, FColor::Green, true);
	//					DrawDebugSphere(GetWorld(), VMid, 10.f, 5, FColor::Yellow, true);
	//					DrawDebugSphere(GetWorld(), VEnd, 10.f, 5, FColor::Red, true);
	//					if (VStart != VMid && VStart != VEnd &&
	//						VEnd != VMid &&
	//						VStart != FVector::ZeroVector &&
	//						VMid != FVector::ZeroVector &&
	//						VEnd != FVector::ZeroVector)
	//					{
	//						return true;
	//					}
	//				}
	//			}

	//			StartVert += GetActorForwardVector() * 50.f;
	//			EndVert += GetActorForwardVector() * 50.f;
	//		}
	//		break;
	//	}
	//}

	return false;
}


void AMotionWarpingCharacter::AfterVault()
{
	GetWorldTimerManager().ClearTimer(AfterVaultTH);
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	SetActorEnableCollision(true);
	bUseControllerRotationYaw = true;
}


void AMotionWarpingCharacter::Jump()
{
	FVector VStart, VMid, VEnd;

	if (bEButtonPressed)// && WarpingComponent)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("WarpingComponent == null"));
		//if(Vault(VStart, VMid, VEnd))
		//{
		//	FMotionWarpingTarget TargetStart;
		//	TargetStart.Name = "VStart";
		//	TargetStart.Location = VStart;
		//	TargetStart.Rotation = GetActorRotation();
		//	WarpingComponent->AddOrUpdateWarpTarget(TargetStart);

		//	FMotionWarpingTarget TargetMid;
		//	TargetMid.Name = "VMid";
		//	TargetMid.Location = VMid;
		//	TargetMid.Rotation = GetActorRotation();
		//	WarpingComponent->AddOrUpdateWarpTarget(TargetMid);

		//	FMotionWarpingTarget TargetEnd;
		//	TargetEnd.Name = "VEnd";
		//	TargetEnd.Location = VEnd;
		//	TargetEnd.Rotation = GetActorRotation();
		//	WarpingComponent->AddOrUpdateWarpTarget(TargetEnd);

		//	//TODO:
		//	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;

		//	if (!AnimInstance)
		//	{
		//		return;
		//	}
		//	if (!VaultMontage)
		//	{
		//		return;
		//	}

		//	GetWorldTimerManager().SetTimer(AfterVaultTH, this, &AMotionWarpingCharacter::AfterVault, AnimInstance->Montage_Play(VaultMontage), false);
		//	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
		//	SetActorEnableCollision(false);
		//	bUseControllerRotationYaw = false;
		//}
	}
	else
	{
		//default jump
		Super::Jump();
	}
}