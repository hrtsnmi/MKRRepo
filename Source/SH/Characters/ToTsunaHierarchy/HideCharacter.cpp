// Fill out your copyright notice in the Description page of Project Settings.


#include "HideCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
//#include "DrawDebugHelpers.h"


void AHideCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GoToCoverWhileHit();
}

AHideCharacter::AHideCharacter()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box To Find Cover"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetBoxExtent(FVector(GetCapsuleComponent()->GetScaledCapsuleRadius() + 5.f));
}

void AHideCharacter::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHideCharacter::BoxComponentBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHideCharacter::BoxComponentEndOverlap);
}

void AHideCharacter::BoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
		{ FindCollision }, QueryParams);

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

		if (bIsNotCoverEndRight)
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

void AHideCharacter::BoxComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor ||
		this == OtherActor ||
		!OtherActor->ActorHasTag(SeatchTag))
		return;

	bCanHide = false;
}

void AHideCharacter::GoToCoverWhileHit()
{
	if (bNeedToReachCover)
	{
		// add movement 
		AddMovementInput(AttachLocation - GetActorLocation());
	}
}

void AHideCharacter::HideSwitcher_Implementation(const FInputActionValue& Value)
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
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHideCharacter::BoxComponentBeginOverlap);
		BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHideCharacter::BoxComponentEndOverlap);
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
			Box1 = GetWorld()->SpawnActor<AActor>(Border, RightEndPoint - AttachLocation, Right.Rotation());
			Box2 = GetWorld()->SpawnActor<AActor>(Border, LeftEndPoint - AttachLocation, Right.Rotation());
		}
		AttachLocation += GetActorLocation();
		GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHideCharacter::OnCapsuleHit);
		break;
	default:
		break;
	}
}

void AHideCharacter::SetHideData_Implementation(EHideStates State)
{
	HideState = State;
}

EHideStates AHideCharacter::ReciveHideData_Implementation()
{
	return HideState;
}

const FVector AHideCharacter::ReturnDirection(const FRotator& YawRotation, EAxis::Type coord) const
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

void AHideCharacter::OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor ||
		this == OtherActor ||
		!OtherActor->ActorHasTag(SeatchTag))
		return;

	bNeedToReachCover = false;

	GetCapsuleComponent()->OnComponentHit.RemoveAll(this);
}