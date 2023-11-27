// Fill out your copyright notice in the Description page of Project Settings.


#include "HideCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Public/CustomMovementComponent.h"
//#include "DrawDebugHelpers.h"
#include "../DebugHelper.h"


void AHideCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GoToCoverWhileHit();
}

void AHideCharacter::Move(const FInputActionValue& Value)
{
	if (HideState == EHideStates::Hide)
	{
		if (GetCustomMovementComponent()->IsActiveMontage())
		{
			return;
		}

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = ReturnDirection(YawRotation, EAxis::X);
		const FVector RightDirection = ReturnDirection(YawRotation, EAxis::Y);
		// add movement 
		const FVector ResultDirection =
			ForwardDirection * Value.Get<FVector2D>().Y + RightDirection * Value.Get<FVector2D>().X;
		const float dotResult = FVector::DotProduct(Right, ResultDirection);
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + YawRotation.Vector() * 50.f, 20, FColor::Magenta);

		const float dotForMontage = FVector::DotProduct(GetActorForwardVector(), YawRotation.Vector());

		if (dotForMontage < 0.f)
		{
			const float dotSelectTurnLeft = FVector::DotProduct(CoverNormal, GetActorRightVector());

			if (dotSelectTurnLeft > 0.f)
			{
				GetCustomMovementComponent()->PlayTurnMontage("TurnRight");
			}
			else
			{
				GetCustomMovementComponent()->PlayTurnMontage();
			}

			return;
		}

		const bool UseRight = (dotResult > 0.f);
		UpdateBorderLocation(UseRight);

		if (UseRight)
		{
			BoxRight->SetActorLocation(RightEndPoint);
		}
		else
		{
			BoxLeft->SetActorLocation(LeftEndPoint);
		}

		TArray<AActor*> borders;
		bool bCanMove = true;
		GetOverlappingActors(borders, Border);
		if (borders.Num() > 0)
		{
			const AActor* border = *borders.begin();

			if (border == BoxRight)
			{
				Debug::Print(*border->GetName(), FColor::Red, 1);
				bCanMove = !UseRight;
			}

			if (border == BoxLeft)
			{
				Debug::Print(*border->GetName(), FColor::Red, 2);
				bCanMove = UseRight;
			}
		}

		if (bCanMove)
		{
			AddMovementInput(ForwardDirection, Value.Get<FVector2D>().Y);
			AddMovementInput(RightDirection, Value.Get<FVector2D>().X);
		}

		return;
	}

	Super::Move(Value);
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
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHideCharacter::OnCapsuleHit);
}

void AHideCharacter::BoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACrouchCharacter::BoxComponentEndOverlap);
	if (!OtherActor ||
		this == OtherActor)// ||
		//!OtherActor->ActorHasTag(SeatchTag))
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
		FromHide();
		break;
	case EHideStates::Visible:
		ToHide();
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
		const float dotResult = FVector::DotProduct(FRotationMatrix(YawRotation).GetUnitAxis(coord), Right);
		FVector Result;
		switch (coord)
		{
		case EAxis::X:
		case EAxis::Y:
			Result = dotResult * Right;
			break;
		default:
			break;
		}

		return Result;
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

	world->LineTraceSingleByObjectType(HitCover, GetActorLocation(), CoverLocation,
		{ FindCollision.GetValue()}, QueryParams);

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
			{ FindCollision.GetValue() }, QueryParams);
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

	FHitResult HitCover;

	const FVector StartPoint = GetActorLocation() + DistanceToCover;
	//Debug::DrawSphere(world, StartPoint, FColor::White);

	FVector NewLocation = FVector::ZeroVector;
	TraceToCover(this, GetWorld(), GetActorLocation(), StartPoint, HitCover,
		NewLocation, SeatchTag, FindCollision.GetValue());
	if (NewLocation == FVector::ZeroVector)
	{
		return;
	}

	if (bUpdateRightBorder)
	{
		RightEndPoint = NewLocation;
	}
	else
	{
		LeftEndPoint = NewLocation;
	}

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
				bUpdateRightBorder ? (RightEndPoint) : (LeftEndPoint), SeatchTag, FindCollision.GetValue());
		}
	}
}

void AHideCharacter::ToHide()
{
	HideState = EHideStates::Hide;
	BoxComp->OnComponentBeginOverlap.RemoveAll(this);
	BoxComp->OnComponentEndOverlap.RemoveAll(this);
	//bNeedToReachCover = true;
	if (Border)
	{
		BoxRight = GetWorld()->SpawnActor<AActor>(Border, RightEndPoint - DistanceToCover, Right.Rotation());
		BoxLeft = GetWorld()->SpawnActor<AActor>(Border, LeftEndPoint - DistanceToCover, Right.Rotation());
	}
	AttachLocation = GetActorLocation() - DistanceToCover;
	bUseControllerRotationYaw = false;
	GetCustomMovementComponent()->bUseSeparateBrakingFriction = false;

	const float dotForMontage = FVector::DotProduct(GetActorForwardVector(), Right);

	SetActorRotation(dotForMontage > 0.f ? Right.Rotation() : (-Right).Rotation());
}

void AHideCharacter::FromHide()
{
	HideState = EHideStates::Visible;
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHideCharacter::BoxComponentBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHideCharacter::BoxComponentEndOverlap);
	bCanHide = false;
	bNeedToReachCover = false;
	if (BoxRight)
	{
		BoxRight->Destroy();
		BoxRight = nullptr;
	}
	if (BoxLeft)
	{
		BoxLeft->Destroy();
		BoxLeft = nullptr;
	}
	bUseControllerRotationYaw = true;
	GetCustomMovementComponent()->bUseSeparateBrakingFriction = true;

	DistanceToCover = CoverNormal = Right = RightEndPoint = LeftEndPoint = FVector::UpVector;
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

void TraceToCover(AActor* Owner, UWorld* Context, const FVector& TraceStart, const FVector& TraceEnd,
	FHitResult& TraceHit, FVector& UpdateLocation, const FName& Tag, ECollisionChannel FindCollision)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	bool bIsNotCoverEnd = Context->LineTraceSingleByObjectType(TraceHit, TraceStart, TraceEnd,
		{ FindCollision }, QueryParams);
	if (bIsNotCoverEnd && TraceHit.GetActor()->ActorHasTag(Tag))
	{
		//sDebug::DrawSphere(Context, TraceEnd, FColor::Red);
		UpdateLocation = TraceHit.ImpactPoint;
	}
}
