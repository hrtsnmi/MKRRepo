// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowCharacter.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/RectLightComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "../../QuantumtKnife.h"
#include "NiagaraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"

AThrowCharacter::AThrowCharacter()
{
	//SetUpKnifeLocation
	Knife = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Knife"));;
	KnifeLocation = CreateDefaultSubobject<USceneComponent>(TEXT("KnifeLocation"));
	RectLightComp = CreateDefaultSubobject<URectLightComponent>(TEXT("RectLightComponent"));
	NS_LeakParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Leak Particles"));
	
	KnifeLocation->SetupAttachment(GetMesh(), FName("KnifeLocation"));
	RectLightComp->SetupAttachment(KnifeLocation);
	Knife->SetupAttachment(KnifeLocation);
	RectLightComp->SetVisibility(false);
	NS_LeakParticles->SetupAttachment(GetMesh());
	
	KnifeSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Knife Spline"));;
	KnifeSpline->SetupAttachment(RootComponent);
	
	EndSpline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("End Spline Mesh"));;
	EndSpline->SetupAttachment(RootComponent);
	EndSpline->SetVisibility(false);
	
	SpawnTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Color Timeline"));
	ProgressFunction.BindUFunction(this, FName("HandleSpawnProgress"));
	SpawnTimelineFinishedEvent.BindUFunction(this, FName("SpawnTimelineFinishedFunction"));
	
	//Predict path
	PredictParams.bTraceWithCollision = true;
	PredictParams.ProjectileRadius = 5.f;
	PredictParams.bTraceWithChannel = true;
	PredictParams.TraceChannel = ECollisionChannel::ECC_Visibility;
	PredictParams.ActorsToIgnore = { this };
	PredictParams.DrawDebugType = EDrawDebugTrace::None;
}

void AThrowCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (SpawnCurveFloat)
	{
		SpawnTimeline->AddInterpFloat(SpawnCurveFloat, ProgressFunction, FName("Alpha"));
		SpawnTimeline->SetTimelineFinishedFunc(SpawnTimelineFinishedEvent);
		SpawnTimeline->SetLooping(false);
	}
}

void AThrowCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		//Throwing
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Completed, this, &AThrowCharacter::Throw);
		//Throwing
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &AThrowCharacter::ShowParticlePath);
		//Throwing
		EnhancedInputComponent->BindAction(KnifeBackAction, ETriggerEvent::Started, this, &AThrowCharacter::SpawnKnifeBack);

	}
}


void AThrowCharacter::Throw(const FInputActionValue& Value)
{
	ClearSpline(SplineMeshes, KnifeSpline);
	
	if (SpawnTimeline->IsActive()) return;
	
	if (bWasThrown)
	{
		if (!ThownProjectile) return;

		//Light
		NS_LeakParticles->Activate();
		//Starting the timeline...
		SpawnTimeline->PlayFromStart();
	}
	else
	{
		EndSpline->SetVisibility(false);
		if (!ThrowMontage) return;
		if (GetWorldTimerManager().IsTimerActive(ThrowMontageTimer)) return;
		float throwingTime = PlayAnimMontage(ThrowMontage, ThrowRate) * 0.45f;
		GetWorldTimerManager().SetTimer(ThrowMontageTimer, FTimerDelegate::CreateLambda([this]
			{
				Knife->SetVisibility(false);

				if (!KnifeProj) return;

				const float CurrCharacterSpeed =
					FVector::DotProduct(GetActorForwardVector(), GetVelocity()) < 0 ?
					(-GetVelocity().Size()) :
					(GetVelocity().Size());
				ThownProjectile = GetWorld()->SpawnActorDeferred<AQuantumtKnife>
					(KnifeProj, ThownTransform, this);
				if (ThownProjectile)
				{
					ThownProjectile->AddOwnerSpeed() = CurrCharacterSpeed + ThownProjectile->StartSpeed;
					ThownProjectile->FinishSpawning(ThownTransform);
				}
			}), throwingTime, false);
		bWasThrown = true;
	}
}

void AThrowCharacter::ShowParticlePath(const FInputActionValue& Value)
{
	if (SpawnTimeline->IsActive()) return;
	
	if (bWasThrown) return;
	
	const FVector WorldSpawnLocation = UKismetMathLibrary::TransformLocation(GetActorTransform(), ThownLocation);
	const FVector WhereCharacterLooks = FRotator(GetFollowCamera()->GetComponentRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll).Vector();

	//DrawDebugDirectionalArrow(GetWorld(), WorldSpawnLocation, WorldSpawnLocation + WhereCharacterLooks * 500.f, 50.f, FColor::Red);// false, 5.f);
	//DrawDebugSphere(GetWorld(), WorldSpawnLocation, 10.f, 10, FColor::Orange);

	ThownTransform.SetLocation(WorldSpawnLocation);
	ThownTransform.SetRotation(FRotator(GetFollowCamera()->GetComponentRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll).Quaternion());

	FPredictProjectilePathResult PredictResult;

	PredictParams.StartLocation = WorldSpawnLocation;
	PredictParams.LaunchVelocity = 1000.f * WhereCharacterLooks + GetVelocity();

	ClearSpline(SplineMeshes, KnifeSpline);
	SplineMeshes.Reserve(PredictResult.PathData.Num());

	bool CanPredictKnifePagh = UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResult);

	if (OnAddSplineMeshAtIndex.IsBound())
	{
		for (int32 i = 0; i < PredictResult.PathData.Num(); ++i)
		{
			KnifeSpline->AddSplinePoint(PredictResult.PathData[i].Location, ESplineCoordinateSpace::Local);
		}
		for (int32 i = 0; i < PredictResult.PathData.Num() - 1; ++i)
		{
			//ExecuteDelegate for Add mesh in Blueprint
			OnAddSplineMeshAtIndex.Broadcast(i);
		}
	}

	KnifeSpline->SetSplinePointType(PredictResult.PathData.Num() - 1, ESplinePointType::CurveClamped);
	EndSpline->SetWorldLocation(PredictResult.PathData.Last().Location);
	EndSpline->SetVisibility(true);
}

void AThrowCharacter::SpawnKnifeBack(const FInputActionValue& Value)
{
	if (bWasThrown)
	{
		//PlayTimeline for Knife
		bWasThrown = false;
		Knife->SetVisibility(true);
		if (ThownProjectile)
		{
			ThownProjectile->Destroy();
		}
	}

	//ActivateEmiter for 0.4 sec
	NS_LeakParticles->Activate();
	GetWorldTimerManager().SetTimer(DeactivateNiagaraTimer, FTimerDelegate::CreateLambda([this]
		{
			NS_LeakParticles->Deactivate();
		}), 0.4f, false);
}

void AThrowCharacter::HandleSpawnProgress(float Value)
{
	//Dissapearing
	GetMesh()->SetScalarParameterValueOnMaterials(ParamaterName, Value);
	NS_LeakParticles->SetNiagaraVariableFloat(ParamaterName.ToString(), Value);
}

//Procces Exid from Timeline
void AThrowCharacter::SpawnTimelineFinishedFunction()
{
	//UE_LOG(LogTemp, Warning, TEXT("SpawnTimelineFinishedFunction"));
	if (bWasThrown)
	{
		//Actor disapered and spawn to new loc
		bWasThrown = false;

		Knife->SetVisibility(true);
		if (ThownProjectile)
		{
			ThownProjectile->Destroy();
			SetActorLocation(ThownProjectile->GetActorLocation());
			FRotator FNewRotator = GetActorRotation();
			FNewRotator.Yaw = GetControlRotation().Yaw;
			SetActorRotation(FNewRotator);
			//GetController()->SetActorRotation(FNewRotator);
		}
		SpawnTimeline->Reverse();
	}
	else
	{
		GetWorldTimerManager().SetTimer(DeactivateNiagaraTimer, FTimerDelegate::CreateLambda([this]
			{
				NS_LeakParticles->Deactivate();
			}), 3.3f, false);
	}
}

//Clear Spline Points
void ClearSpline(TArray<USplineMeshComponent*>& SplineMeshes, USplineComponent* KnifeSpline)
{
	if (SplineMeshes.Num() > 0)
	{
		for (USplineMeshComponent* SplineMeshComponent : SplineMeshes)
		{
			SplineMeshComponent->DestroyComponent();
		}
		SplineMeshes.Empty();
	}
	KnifeSpline->ClearSplinePoints();
}