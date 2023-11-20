// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

ACrouchCharacter::ACrouchCharacter()
{

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box To Find Cover"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetBoxExtent(FVector(GetCapsuleComponent()->GetScaledCapsuleRadius() + 5.f));
	
}

void ACrouchCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GoToCoverWhileHit();
}

void ACrouchCharacter::BeginPlay()
{
	Super::BeginPlay();

	FPlayerMovementInfo SpeedData;
	SpeedData.MaxWalkSpeed = 250.f;
	SpeedData.MaxAcceleration = 500.f;
	SpeedData.BrakingDeceleration = 500.f;
	CrouchSpeedData.Add(ESpeedStates::Joging, SpeedData);

	SpeedData.MaxWalkSpeed = 180.f;
	SpeedData.MaxAcceleration = 360.f;
	SpeedData.BrakingDeceleration = 360.f;
	CrouchSpeedData.Add(ESpeedStates::Walking, SpeedData);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACrouchCharacter::BoxComponentBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACrouchCharacter::BoxComponentEndOverlap);

	CharacterCrouchState == ECrouchStates::Stand ?
		EntryExitState = EEntryExitStates::Enrty : EntryExitState = EEntryExitStates::Exit;

}

void ACrouchCharacter::CrouchSwitcher_Implementation(const FInputActionValue& Value)
{
	ECrouchStates NewState = CharacterCrouchState;
	FPlayerMovementInfo* NewMovementInfo = nullptr;

	switch (CharacterCrouchState)
	{
	case ECrouchStates::Stand:
		NewState = ECrouchStates::Crouch;
		break;
	case ECrouchStates::Crouch:
		NewState = ECrouchStates::Stand;
		break;
	default:
		break;
	}
	SetCrouchData_Implementation(NewState);
}

void ACrouchCharacter::SetCrouchData_Implementation(ECrouchStates State)
{
	CharacterCrouchState = State;
	UpdateMovementDependsOnState(GetPlayerMovementInfo(), GetCharacterMovement());
}

FPlayerMovementInfo* ACrouchCharacter::GetPlayerMovementInfo()
{
	switch (CharacterCrouchState)
	{
	case ECrouchStates::Stand:
		return Super::GetPlayerMovementInfo();
	case ECrouchStates::Crouch:
		return CrouchSpeedData.Find(CharacterSpeedState);;
	default:
		return nullptr;
	}
}

void ACrouchCharacter::BoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACrouchCharacter::BoxComponentEndOverlap);
	if (!OtherActor ||
		this == OtherActor ||
		!OtherActor->ActorHasTag(SeatchTag))
		return;
	
	UWorld* world = GetWorld();
	
	FHitResult HitCover;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	ECollisionChannel FindCollision = ECollisionChannel::ECC_WorldStatic;

	world->LineTraceSingleByObjectType(HitCover, GetActorLocation(), OtherActor->GetActorLocation(),
		{ FindCollision}, QueryParams);

	FVector Normal = HitCover.Normal;
	FVector DistanceToCover = Normal * FVector::DotProduct(Normal, HitCover.ImpactPoint - GetActorLocation());

	DrawDebugLine(world, GetActorLocation(), GetActorLocation() + DistanceToCover, FColor::Red, false, 5.f);

	//DrawDebugSphere(world, HitCover.ImpactPoint, 10.f, 10, FColor::Green, false, 5.f);
	DrawDebugDirectionalArrow(world, HitCover.ImpactPoint,
		HitCover.ImpactPoint + Normal * 100.f, 20, FColor::Blue, false, 5.f);

	const int32 stepAmount = 5;
	int32 verticalStep = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / stepAmount;
	for (int32 i = 0; i < stepAmount; ++i)
	{
		FVector StartLocation = GetActorLocation();
		StartLocation.Z -= verticalStep * i;

		bool bIsGoodCover = world->LineTraceSingleByObjectType(HitCover, StartLocation, StartLocation - DistanceToCover,
			{ FindCollision }, QueryParams);

		if (!bIsGoodCover) break;
		//DrawDebugLine(world, StartLocation, StartLocation - DistanceToCover, FColor::Yellow, false, 5.f);
	}

	float horizonStep = GetCapsuleComponent()->GetScaledCapsuleRadius() * 0.25f;
	bool bIsNotCoverEndRight = true;
	bool bIsNotCoverEndLeft = true;
	Right = FVector::CrossProduct(Normal, GetActorUpVector());
	DrawDebugLine(world, GetActorLocation(), GetActorLocation() + Right * 100.f, FColor::Yellow, false, 5.f);
	Right.Normalize();
	FVector EndRight;
	FVector EndLeft;
	

	for (int32 i = 0; bIsNotCoverEndRight || bIsNotCoverEndLeft; ++i)
	{
		EndRight = GetActorLocation() + DistanceToCover * 2.f + Right * horizonStep * i;
		EndLeft = GetActorLocation() + DistanceToCover * 2.f - Right * horizonStep * i;

		if(bIsNotCoverEndRight)
		{
			bIsNotCoverEndRight = world->LineTraceSingleByObjectType(HitCover, GetActorLocation(), EndRight,
				{ FindCollision }, QueryParams);

			if (bIsNotCoverEndRight)
			{
				RightEndPoint = HitCover.ImpactPoint;
			}
		}

		if (bIsNotCoverEndLeft)
		{
			bIsNotCoverEndLeft = world->LineTraceSingleByObjectType(HitCover, GetActorLocation(), EndLeft,
				{ FindCollision }, QueryParams);

			if (bIsNotCoverEndLeft)
			{
				LeftEndPoint = HitCover.ImpactPoint;
			}
		}
	}

	DrawDebugSphere(world, RightEndPoint, 10.f, 10, FColor::Green, false, 5.f);
	DrawDebugSphere(world, LeftEndPoint, 10.f, 10, FColor::Green, false, 5.f);

	if ((RightEndPoint - LeftEndPoint).Size() < GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.0f)
	{
		return;
	}

	bCanHide = true;
	AttachLocation = DistanceToCover;
}

void ACrouchCharacter::BoxComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor ||
		this == OtherActor ||
		!OtherActor->ActorHasTag(SeatchTag))
		return;

	bCanHide = false;
}

void ACrouchCharacter::GoToCoverWhileHit()
{
	if (bNeedToReachCover)
	{
		// add movement 
		AddMovementInput(AttachLocation - GetActorLocation());
	}
}

const FVector ACrouchCharacter::ReturnDirection(const FRotator& YawRotation, EAxis::Type coord) const
{
	// TODO: insert return statement here
	if (HideState == EHideStates::Hide)
	{
		switch (coord)
		{
		case EAxis::X:
		case EAxis::Y:
			return FVector::DotProduct(FRotationMatrix(YawRotation).GetUnitAxis(coord), Right) * Right;;
		default:
			return FVector::ZeroVector;
			break;
		}
	}
	else
	{
		return Super::ReturnDirection(YawRotation, coord);
	}
}

void ACrouchCharacter::HideSwitcher_Implementation(const FInputActionValue& Value)
{
	if (!bCanHide)
	{
		return;
	}
	if (CharacterCrouchState != ECrouchStates::Crouch)
	{
		return;
	}

	switch (HideState)
	{
	case EHideStates::Hide:
		HideState = EHideStates::Visible;
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACrouchCharacter::BoxComponentBeginOverlap);
		BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACrouchCharacter::BoxComponentEndOverlap);
		bCanHide = false;
		bNeedToReachCover = false;
		if (Box1)
		{
			Box1->Destroy();
		}
		if (Box2)
		{
			Box2->Destroy();
		}
		GetCapsuleComponent()->OnComponentHit.RemoveAll(this);
		break;
	case EHideStates::Visible:
		HideState = EHideStates::Hide;
		BoxComp->OnComponentBeginOverlap.RemoveAll(this);
		BoxComp->OnComponentEndOverlap.RemoveAll(this);
		bNeedToReachCover = true;
		if (Border)
		{
			Box1 = GetWorld()->SpawnActor<AActor>(Border, RightEndPoint - AttachLocation,Right.Rotation());
			Box2 = GetWorld()->SpawnActor<AActor>(Border, LeftEndPoint - AttachLocation, Right.Rotation());
		}
		AttachLocation += GetActorLocation();
		GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACrouchCharacter::OnCapsuleHit);
		break;
	default:
		break;
	}
}

void ACrouchCharacter::SetHideData_Implementation(EHideStates State)
{
	HideState = State;
}

EHideStates ACrouchCharacter::ReciveHideData_Implementation()
{
	return HideState;
}

ECrouchStates ACrouchCharacter::ReciveCrouchData_Implementation()
{
	return CharacterCrouchState;
}

void ACrouchCharacter::SetEntryExitStateCrouchData_Implementation(EEntryExitStates State)
{
	EntryExitState = State;
}

EEntryExitStates ACrouchCharacter::ReciveCrouchEntryData_Implementation()
{
	return EntryExitState;
}

void ACrouchCharacter::OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor ||
		this == OtherActor ||
		!OtherActor->ActorHasTag(SeatchTag))
		return;

	bNeedToReachCover = false;
}