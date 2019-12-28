// Fill out your copyright notice in the Description page of Project Settings.

#include "Missile.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetLinearDamping(1.f);
	Mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &AMissile::OnHit);
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	FVector Direction;
	if (Target && Timer > TimeBeforeTracking)
	{
		Direction = Target->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
	}
	else
	{
		FRotator Rotator = GetActorRotation();
		Direction = FRotationMatrix(Rotator).GetUnitAxis(EAxis::X);
	}

	Mesh->SetPhysicsLinearVelocity(Direction * Speed);
	SetActorRotation(Direction.Rotation());

	if (Timer > TimeToLive)
	{
		Destroy();
	}
}

void AMissile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Target == OtherActor)
	{
		OtherActor->Destroy();
	}

	Destroy();
}