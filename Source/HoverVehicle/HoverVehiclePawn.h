// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Missile.h"

#include "GameFramework/Pawn.h"
#include "HoverVehiclePawn.generated.h"

UCLASS()
class HOVERVEHICLE_API AHoverVehiclePawn : public APawn
{
	GENERATED_BODY()

private:
	float ThrustInput = 0.f;
	float YawInput = 0.f;
	float LookMouseInput = 0.f;
	float LookGamepadInput = 0.f;
	float LastFired = TNumericLimits<float>::Max();

	bool bUsingGamepad = true;
	bool bLeftMissileLastFired = true;

	TArray<AActor*> TargetsInView;
	AActor* CurrentTarget = nullptr;
	int CurrentTargetIndex = 0;

public:
	// Sets default values for this pawn's properties
	AHoverVehiclePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float YawMaxTorque;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ThrustMaxForce;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HoverMaxForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HoverMaxHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float LookMaxSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FireCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ShootingDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* CenterRaycast;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* LeftMissileLauncher;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* RightMissileLauncher;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMissile> MissileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> TargetClass;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Thrust(float Value);

	UFUNCTION()
	void Turn(float Value);

	UFUNCTION()
	void Look(float Value);

	UFUNCTION()
	void LookGamepad(float Value);

	UFUNCTION()
	void FireMissile();

	UFUNCTION()
	void CycleTarget();
	
	UFUNCTION()
	TArray<AActor*> GetTargets();

	UFUNCTION()
	AActor* GetCurrentTarget();
};
