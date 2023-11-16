// Fill out your copyright notice in the Description page of Project Settings.


#include "QuantumtKnife.h"
#include "Components/RectLightComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

void AQuantumtKnife::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

// Sets default values
AQuantumtKnife::AQuantumtKnife()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KnifeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Projectile Mesh"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	
	RootComponent = BoxComponent;
	KnifeMesh->SetupAttachment(RootComponent);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	NS_LeakParticlesProj = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Leak Particles"));
	NS_LeakParticlesProj->SetupAttachment(KnifeMesh);

	if (LaunchSound)
		UGameplayStatics::PlaySoundAtLocation(this,
			LaunchSound,
			GetActorLocation());


	ProjectileMovementComp->InitialSpeed = 1000.f;
}

// Called when the game starts or when spawned
void AQuantumtKnife::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuantumtKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//SetActorRotation(GetVelocity().Rotation());

	
	DrawDebugSphere(GetWorld(), GetActorLocation(), 500.f, 24, FColor::Red);

	
	
}

void AQuantumtKnife::UseTimeLineParticle(const FString& ParamaterName, float Value)
{
	NS_LeakParticlesProj->SetNiagaraVariableFloat(ParamaterName, Value);
}

void AQuantumtKnife::ActivateParticle(bool bActivate)
{
	if (bActivate)
	{
		NS_LeakParticlesProj->Activate();
	}
	else
	{
		NS_LeakParticlesProj->Deactivate();
	}

}

float& AQuantumtKnife::AddOwnerSpeed()
{
	return ProjectileMovementComp->InitialSpeed;
}
