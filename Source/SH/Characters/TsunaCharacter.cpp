// Fill out your copyright notice in the Description page of Project Settings.


#include "TsunaCharacter.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/RectLightComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "../QuantumtKnife.h"
#include "NiagaraComponent.h"

ATsunaCharacter::ATsunaCharacter()
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

	SpawnTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Color Timeline"));
	ProgressFunction.BindUFunction(this, FName("HandleSpawnProgress"));
	SpawnTimelineFinishedEvent.BindUFunction(this, FName("SpawnTimelineFinishedFunction"));
}

void ATsunaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnCurveFloat)
	{
		SpawnTimeline->AddInterpFloat(SpawnCurveFloat, ProgressFunction, FName("Alpha"));
		SpawnTimeline->SetTimelineFinishedFunc(SpawnTimelineFinishedEvent);
		SpawnTimeline->SetLooping(false);
	}

}

void ATsunaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Throwing
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Completed, this, &ATsunaCharacter::Throw);
		//Throwing
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &ATsunaCharacter::ShowParticlePath);

		//Throwing
		EnhancedInputComponent->BindAction(KnifeBackAction, ETriggerEvent::Started, this, &ATsunaCharacter::SpawnKnifeBack);
		
	}
}

void ATsunaCharacter::Throw(const FInputActionValue& Value)
{
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
		if (!ThrowMontage) return;

		if (GetWorldTimerManager().IsTimerActive(ThrowMontageTimer)) return;

		float throwingTime = PlayAnimMontage(ThrowMontage, ThrowRate);

		GetWorldTimerManager().SetTimer(ThrowMontageTimer, FTimerDelegate::CreateLambda([this]
			{
				Knife->SetVisibility(false);

				FTransform ProjectileTransform;
				ProjectileTransform.SetLocation(KnifeLocation->GetComponentLocation());
				
				FRotator NewRotator = GetActorRotation();
				NewRotator.Roll = GetFollowCamera()->GetComponentRotation().Roll;
				ProjectileTransform.SetRotation(NewRotator.Quaternion());

				//ProjectileTransform.SetScale3D(FVector(ProjectileScale));
			
				if (!KnifeProj) return;
				ThownProjectile = GetWorld()->SpawnActorDeferred<AQuantumtKnife>
					(KnifeProj, ProjectileTransform, this);
				UGameplayStatics::FinishSpawningActor(ThownProjectile, ProjectileTransform);

			}), throwingTime * 0.2f, false);

		bWasThrown = true;
	}
}

void ATsunaCharacter::ShowParticlePath(const FInputActionValue& Value)
{
}

void ATsunaCharacter::SpawnKnifeBack(const FInputActionValue& Value)
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


//To Proccces Spawn
void ATsunaCharacter::HandleSpawnProgress(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("HandleSpawnProgress: %f"), Value);
	//Dissapearing
	GetMesh()->SetScalarParameterValueOnMaterials(ParamaterName, Value);
	NS_LeakParticles->SetNiagaraVariableFloat(ParamaterName.ToString(), Value);

	/*UseTimeLineParticle(ParamaterName.ToString(), Value);

	ActivateParticle*/
}


//Procces Exid from Timeline
void ATsunaCharacter::SpawnTimelineFinishedFunction()
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