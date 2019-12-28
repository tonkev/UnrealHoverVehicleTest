// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HoverVehicleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HOVERVEHICLE_API AHoverVehicleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;
};
