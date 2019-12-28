// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Missile.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class HOVERVEHICLE_API ATurret : public AActor
{
	GENERATED_BODY()

private:
	APawn* PlayerPawn;

	float LastFired = TNumericLimits<float>::Max();

public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FireCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ShootingDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* MissileLauncher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMissile> MissileClass;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
