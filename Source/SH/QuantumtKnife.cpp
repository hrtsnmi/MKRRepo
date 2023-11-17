// Fill out your copyright notice in the Description page of Project Settings.


#include "QuantumtKnife.h"
#include "Components/RectLightComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"
#include "Characters/ToTsunaHierarchy/LookAtKnifeInterface.h"

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
	
	GetWorldTimerManager().SetTimer(LightTimerDelegate, FTimerDelegate::CreateLambda([this]
		{
			Light();
		}), 0.5f, true,5.f);
}

void AQuantumtKnife::Light()
{

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), ULookAtKnifeInterface::StaticClass(), Actors);

	for (const AActor* Actor : Actors)
	{
		if((Actor->GetActorLocation() - GetActorLocation()).Size() <= 500.f)
		{
			ILookAtKnifeInterface::Execute_SetKnifeLoaction((UObject*)Actor, GetActorLocation());
		}
	}
	
}

// Called every frame
void AQuantumtKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	SetActorRotation(GetVelocity().Rotation());
	
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

void AQuantumtKnife::StopLight()
{
	GetWorldTimerManager().ClearTimer(LightTimerDelegate);
	Destroy();
}

