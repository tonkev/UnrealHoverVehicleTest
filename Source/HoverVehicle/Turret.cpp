// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);

	MissileLauncher = CreateDefaultSubobject<USceneComponent>(TEXT("MissileLauncher"));
	MissileLauncher->AttachTo(Mesh);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn && LastFired > FireCooldown)
	{
		FVector PlayerVelocity = PlayerPawn->GetVelocity();
		FVector Difference = PlayerPawn->GetActorLocation() - (GetActorLocation() + MissileLauncher->GetRelativeLocation());

		if (Difference.Size() < ShootingDistance)
		{
			float MissileSpeed = MissileClass.GetDefaultObject()->Speed;

			float A = FVector::DotProduct(PlayerVelocity, PlayerVelocity) - (MissileSpeed * MissileSpeed);
			float B = 2.f * FVector::DotProduct(Difference, PlayerVelocity);
			float C = FVector::DotProduct(Difference, Difference);

			float T = -1.f;
			float B4AC = (B * B) - (4 * A * C);
			if (B4AC > 0)
			{
				B4AC = sqrtf(B4AC);
				float T1 = (-B + B4AC) / (2.f * A);
				float T2 = (-B - B4AC) / (2.f * A);

				if (T1 >= 0 && T1 <= T2)
				{
					T = T1;
				}
				else if (T2 > 0)
				{
					T = T2;
				}
			}

			if (T >= 0)
			{
				FVector PlayerDestination = PlayerPawn->GetActorLocation() + (PlayerPawn->GetVelocity() * T);
				FVector Direction = PlayerDestination - (GetActorLocation() + MissileLauncher->GetRelativeLocation());
				Direction.Normalize();

				AMissile* Missile = GetWorld()->SpawnActor<AMissile>(MissileClass, MissileLauncher->GetComponentTransform());
				Missile->SetActorRotation(Direction.Rotation());

				LastFired = 0.f;
			}
		}
	}

	LastFired += DeltaTime;
}

