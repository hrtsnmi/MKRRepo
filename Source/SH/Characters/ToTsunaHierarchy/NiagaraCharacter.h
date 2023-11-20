// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WithCameraCharacter.h"
#include "NiagaraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SH_API ANiagaraCharacter : public AWithCameraCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw|Data", meta = (AllowPrivateAccess = "true"))
		class UNiagaraComponent* NS_LeakParticles;

public:
	ANiagaraCharacter();
};
