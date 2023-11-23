// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingSystemCharacter.h"
#include "DebugHelper.h"
#include "Public/CustomMovementComponent.h"

AClimbingSystemCharacter::AClimbingSystemCharacter()
{
	CustomMovementComponent = Cast<UCustomMovementComponent>(GetCharacterMovement());
}

// Sets default values
AClimbingSystemCharacter::AClimbingSystemCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	
	CustomMovementComponent = Cast<UCustomMovementComponent>(GetCharacterMovement());
}

// Called when the game starts or when spawned
void AClimbingSystemCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AClimbingSystemCharacter::HandleClimbMovementInput(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FVector ForwardDirection = FVector::CrossProduct(
		-CustomMovementComponent->GetClimbableSurfaceNormal(),
		GetActorRightVector()
	);

	const FVector RightDirection = FVector::CrossProduct(
		-CustomMovementComponent->GetClimbableSurfaceNormal(),
		-GetActorUpVector()
	);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AClimbingSystemCharacter::Move(const FInputActionValue& Value)
{
	if (!CustomMovementComponent) return;

	if (CustomMovementComponent->IsClimbing())
	{
		HandleClimbMovementInput(Value);
		return;
	}
}

void AClimbingSystemCharacter::OnClimbActionStarted(const FInputActionValue& Value)
{
	Debug::Print(TEXT("OnClimbActionStarted"));

	if (!CustomMovementComponent) return;

	if (!CustomMovementComponent->IsClimbing())
	{
		CustomMovementComponent->ToggleClimbing(true);
	}
	else
	{
		CustomMovementComponent->ToggleClimbing(false);
	}
}