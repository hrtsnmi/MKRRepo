// Fill out your copyright notice in the Description page of Project Settings.


#include "NiagaraCharacter.h"
#include "NiagaraComponent.h"


ANiagaraCharacter::ANiagaraCharacter()
{
	NS_LeakParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Leak Particles"));
	NS_LeakParticles->SetupAttachment(GetMesh());
}