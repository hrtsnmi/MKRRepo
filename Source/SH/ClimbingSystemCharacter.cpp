// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingSystemCharacter.h"
#include "DebugHelper.h"
#include "Public/CustomMovementComponent.h"

AClimbingSystemCharacter::AClimbingSystemCharacter()
{
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



void AClimbingSystemCharacter::OnClimbActionStarted(const FInputActionValue& Value)
{
	Debug::Print(TEXT("Climb action started"));
}