// Copyright Epic Games, Inc. All Rights Reserved.

#include "SHGameMode.h"
#include "Characters/TsunaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASHGameMode::ASHGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/Character/BP_TsunaCharacter.BP_TsunaCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
