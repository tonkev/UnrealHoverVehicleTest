// Fill out your copyright notice in the Description page of Project Settings.


#include "HoverVehiclePawn.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AHoverVehiclePawn::AHoverVehiclePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
	Mesh->AttachTo(RootComponent);

	CenterRaycast = CreateDefaultSubobject<USceneComponent>(TEXT("CenterRaycast"));
	CenterRaycast->AttachTo(Mesh);

	LeftMissileLauncher = CreateDefaultSubobject<USceneComponent>(TEXT("LeftMissileLauncher"));
	LeftMissileLauncher->AttachTo(Mesh);

	RightMissileLauncher = CreateDefaultSubobject<USceneComponent>(TEXT("RightMissileLauncher"));
	RightMissileLauncher->AttachTo(Mesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachTo(Mesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(SpringArm);
}

// Called when the game starts or when spawned
void AHoverVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHoverVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Turn
	Mesh->AddTorqueInRadians(FVector(0.f, 0.f, 1.f) * YawMaxTorque * YawInput);
	
	//Move
	FRotator Rotator = GetActorRotation();
	FVector Forward = FRotationMatrix(Rotator).GetUnitAxis(EAxis::X);
	Mesh->AddForce(Forward * ThrustMaxForce * ThrustInput);

	//Hover
	FHitResult Hit;
	FVector RayStart = CenterRaycast->GetComponentLocation();
	FVector Up = FVector(0.f, 0.f, 1.f);
	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
	CollisionQueryParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, RayStart, RayStart + (-Up * HoverMaxHeight), ECC_Visibility, CollisionQueryParams))
	{
		Mesh->AddForce(Up * HoverMaxForce * (1 - Hit.Time));
	}

	//Rotate Camera
	FRotator SpringArmRotator = SpringArm->GetRelativeRotation();
	if (bUsingGamepad)
	{
		SpringArmRotator.Yaw = 90.f * LookGamepadInput;
	}
	else
	{
		SpringArmRotator.Yaw += LookMaxSpeed * LookMouseInput * DeltaTime;
		SpringArmRotator.Yaw = FMath::ClampAngle(SpringArmRotator.Yaw, -90.f, 90.f);
	}
	SpringArm->SetRelativeRotation(SpringArmRotator);

	//Update Missile Timer
	LastFired += DeltaTime;

	//Update Available Targets, Change Current Target if no longer in view
	TargetsInView.Empty();
	bool foundCurrentTarget = false;
	AActor* FirstTarget = nullptr;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		int32 ScreenWidth;
		int32 ScreenHeight;
		PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

		for (AActor* Target : GetTargets())
		{
			FVector ScreenLocation;
			if (PlayerController->ProjectWorldLocationToScreenWithDistance(Target->GetActorLocation(), ScreenLocation, false))
			{
				if (0 <= ScreenLocation.X && ScreenLocation.X <= ScreenWidth && 0 <= ScreenLocation.Y && ScreenLocation.Y <= ScreenHeight && ScreenLocation.Z < ShootingDistance)
				{
					TargetsInView.Add(Target);
					if (Target == CurrentTarget)
					{
						foundCurrentTarget = true;
					}
					if (!FirstTarget)
					{
						FirstTarget = Target;
					}
				}
			}
		}
	}

	if (!foundCurrentTarget)
	{
		CurrentTarget = FirstTarget;
		CurrentTargetIndex = 0;
	}
}

// Called to bind functionality to input
void AHoverVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Thrust", this, &AHoverVehiclePawn::Thrust);
	PlayerInputComponent->BindAxis("Turn", this, &AHoverVehiclePawn::Turn);
	PlayerInputComponent->BindAxis("Look", this, &AHoverVehiclePawn::Look);
	PlayerInputComponent->BindAxis("LookGamepad", this, &AHoverVehiclePawn::LookGamepad);
	PlayerInputComponent->BindAction("FireMissile", EInputEvent::IE_Pressed, this, &AHoverVehiclePawn::FireMissile);
	PlayerInputComponent->BindAction("CycleTarget", EInputEvent::IE_Pressed, this, &AHoverVehiclePawn::CycleTarget);
}

void AHoverVehiclePawn::Thrust(float Value)
{
	ThrustInput = Value;
}

void AHoverVehiclePawn::Turn(float Value)
{
	YawInput = Value;
}

void AHoverVehiclePawn::Look(float Value)
{
	LookMouseInput = Value;
	if (Value != 0)
	{
		bUsingGamepad = false;
	}
}

void AHoverVehiclePawn::LookGamepad(float Value)
{
	LookGamepadInput = Value;
	if (Value != 0)
	{
		bUsingGamepad = true;
	}
}

void AHoverVehiclePawn::FireMissile()
{
	if (LastFired > FireCooldown)
	{
		USceneComponent* MissileLauncher = LeftMissileLauncher;
		if (bLeftMissileLastFired)
		{
			MissileLauncher = RightMissileLauncher;
		}
		bLeftMissileLastFired = !bLeftMissileLastFired;

		AMissile* Missile = GetWorld()->SpawnActor<AMissile>(MissileClass, MissileLauncher->GetComponentTransform());
		Missile->Target = CurrentTarget;

		LastFired = 0;
	}
}

void AHoverVehiclePawn::CycleTarget()
{
	if (CurrentTarget)
	{
		CurrentTargetIndex = (CurrentTargetIndex + 1) % TargetsInView.Num();
		CurrentTarget = TargetsInView[CurrentTargetIndex];
	}
}

TArray<AActor*> AHoverVehiclePawn::GetTargets()
{
	TArray<AActor*> Targets;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsA(TargetClass))
		{
			Targets.Add(*ActorItr);
		}
	}
	return Targets;
}

AActor* AHoverVehiclePawn::GetCurrentTarget()
{
	return CurrentTarget;
}