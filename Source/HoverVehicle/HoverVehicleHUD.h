// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HoverVehiclePawn.h"

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HoverVehicleHUD.generated.h"

/**
 * 
 */
UCLASS()
class HOVERVEHICLE_API AHoverVehicleHUD : public AHUD
{
	GENERATED_BODY()

private:
	AHoverVehiclePawn* HoverVehicle;

protected:
	virtual void BeginPlay() override;

	virtual void PostRender() override;
};
