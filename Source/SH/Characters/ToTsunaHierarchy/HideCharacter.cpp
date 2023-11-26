// Fill out your copyright notice in the Description page of Project Settings.


#include "HideCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
//#include "DrawDebugHelpers.h"
#include "../DebugHelper.h"


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

	SutUpHideBorders(OtherActor->GetActorLocation());
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
			Box1 = nullptr;
		}
		if (Box2)
		{
			Box2->Destroy();
			Box2 = nullptr;
		}
		GetCapsuleComponent()->OnComponentHit.RemoveAll(this);
		bUseControllerRotationYaw = true;
		break;
	case EHideStates::Visible:
		HideState = EHideStates::Hide;
		BoxComp->OnComponentBeginOverlap.RemoveAll(this);
		BoxComp->OnComponentEndOverlap.RemoveAll(this);
		//bNeedToReachCover = true;
		if (Border)
		{
			Box1 = GetWorld()->SpawnActor<AActor>(Border, RightEndPoint + DistanceToCover, Right.Rotation());
			Box2 = GetWorld()->SpawnActor<AActor>(Border, LeftEndPoint + DistanceToCover, Right.Rotation());
		}
		AttachLocation = GetActorLocation() - DistanceToCover;
		GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHideCharacter::OnCapsuleHit);

		bUseControllerRotationYaw = false;
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

FVector AHideCharacter::ReturnDirection(const FRotator& YawRotation, EAxis::Type coord)
{
	// TODO: insert return statement here
	if (HideState == EHideStates::Hide)
	{
		const float dotResult = FVector::DotProduct(FRotationMatrix(YawRotation).GetUnitAxis(coord), Right);

		switch (coord)
		{
		case EAxis::X:
		case EAxis::Y:
			return dotResult * Right;;
		default:
			return FVector::ZeroVector;
			break;
		}

		UpdateBorderLocation((dotResult > 0.f));

		FlushPersistentDebugLines(GetWorld());
		Debug::DrawSphere(GetWorld(), RightEndPoint, FColor::Black);
		Debug::DrawSphere(GetWorld(), LeftEndPoint, FColor::Green);
	}
	else
	{
		return Super::ReturnDirection(YawRotation, coord);
	}
}

void AHideCharacter::SutUpHideBorders(const FVector& CoverLocation)
{
	UWorld* world = GetWorld();

	FHitResult HitCover;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	ECollisionChannel FindCollision = ECollisionChannel::ECC_WorldStatic;

	world->LineTraceSingleByObjectType(HitCover, GetActorLocation(), CoverLocation,
		{ FindCollision }, QueryParams);

	CoverNormal = HitCover.Normal;
	DistanceToCover = CoverNormal * FVector::DotProduct(CoverNormal, HitCover.ImpactPoint - GetActorLocation());

	//Debug::DrawDebugArrow(world, GetActorLocation(), DistanceToCover, FColor::Red, 5.f, false);
	//Debug::DrawDebugArrow(world, HitCover.ImpactPoint, Normal, FColor::Blue, 5.f);

	const int32 stepAmount = 5;
	int32 verticalStep = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / stepAmount;

	bool bIsGoodCover = true;
	for (int32 i = 0;
		i < stepAmount && bIsGoodCover;
		++i)
	{
		FVector StartLocation = GetActorLocation();
		StartLocation.Z -= verticalStep * i;

		bIsGoodCover = world->LineTraceSingleByObjectType(HitCover, StartLocation, StartLocation - DistanceToCover,
			{ FindCollision }, QueryParams);
	}

	UpdateBorderLocation(true);
	UpdateBorderLocation(false);
	
	Debug::DrawSphere(world, RightEndPoint, FColor::Black);
	Debug::DrawSphere(world, LeftEndPoint, FColor::Green);

	bCanHide = (RightEndPoint - LeftEndPoint).Size() > (GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.f);
}

void AHideCharacter::UpdateBorderLocation(bool bUpdateRightBorder)
{
	const float divider = 0.25f;
	const float distance = GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.f;
	float currDistance{};
	float horizonStep = GetCapsuleComponent()->GetScaledCapsuleRadius() * divider;
	bool bIsNotCoverEnd = true;
	Right = FVector::CrossProduct(CoverNormal, GetActorUpVector());
	Right.Normalize();
	//Debug::DrawDebugArrow(world, GetActorLocation(), Right, FColor::Red, 5.f);

	const FVector StartPoint = GetActorLocation() + DistanceToCover;
	//Debug::DrawSphere(world, StartPoint, FColor::White);

	if (bUpdateRightBorder)
	{
		RightEndPoint = StartPoint;
	}
	else
	{
		LeftEndPoint = StartPoint;
	}

	FHitResult HitCover;

	FVector EndPoint;

	for (int32 i = 1;
		currDistance < distance;
		++i)
	{
		FVector NextPointTo = Right * horizonStep * i;

		if (currDistance < distance)
		{
			if (bUpdateRightBorder)
			{
				EndPoint = StartPoint - CoverNormal + NextPointTo;
			}
			else
			{
				EndPoint = StartPoint - CoverNormal - NextPointTo;
			}
			
			currDistance = (StartPoint - EndPoint).Size();
		}

		if (currDistance < distance)
		{
			TraceToCover(this, GetWorld(), GetActorLocation(), EndPoint, HitCover,
				bUpdateRightBorder ? (RightEndPoint) : (LeftEndPoint), SeatchTag);
		}
	}
}

void AHideCharacter::OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor ||
		this == OtherActor)
		return;

	if(OtherActor->ActorHasTag(SeatchTag))
	{
		bNeedToReachCover = false;
		GetCapsuleComponent()->OnComponentHit.RemoveAll(this);
	}
}

void TraceToCover(AActor* Owner, UWorld* Context, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& TraceHit, FVector& UpdateLocation, const FName& Tag)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);
	ECollisionChannel FindCollision = ECollisionChannel::ECC_WorldStatic;

	bool bIsNotCoverEnd = Context->LineTraceSingleByObjectType(TraceHit, TraceStart, TraceEnd,
		{ FindCollision }, QueryParams);
	if (bIsNotCoverEnd && TraceHit.GetActor()->ActorHasTag(Tag))
	{
		//sDebug::DrawSphere(Context, TraceEnd, FColor::Red);
		UpdateLocation = TraceHit.ImpactPoint;
	}
}
