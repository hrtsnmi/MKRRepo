// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SHCharacter.h"
#include "../Interfaces/WalkSpeedUpdateInterface.h"
#include "../Interfaces/FollowPatrolPathInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */

UCLASS()
class SH_API AEnemyCharacter : public ASHCharacter, public IFollowPatrolPathInterface
{
	GENERATED_BODY()
	
};
