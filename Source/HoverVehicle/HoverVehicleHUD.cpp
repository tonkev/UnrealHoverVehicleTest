// Fill out your copyright notice in the Description page of Project Settings.

#include "HoverVehicleHUD.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void AHoverVehicleHUD::BeginPlay()
{
	Super::BeginPlay();

	HoverVehicle = Cast<AHoverVehiclePawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
}


void AHoverVehicleHUD::PostRender()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (HoverVehicle && PlayerController)
	{
		for (AActor* Target : HoverVehicle->GetTargets())
		{
			int32 ScreenWidth;
			int32 ScreenHeight;
			PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

			FVector ScreenLocation;
			if (PlayerController->ProjectWorldLocationToScreenWithDistance(Target->GetActorLocation(), ScreenLocation, false))
			{
				ScreenLocation.X = FMath::Clamp(ScreenLocation.X, 0.f, (float)ScreenWidth);
				ScreenLocation.Y = FMath::Clamp(ScreenLocation.Y, 0.f, (float)ScreenHeight);
			}
			else
			{
				ScreenLocation.X = ScreenWidth / 2;
				ScreenLocation.Y = ScreenHeight;
				ScreenLocation.Z = TNumericLimits<float>::Max();
			}

			float Size = FMath::Clamp(50000.f / ScreenLocation.Z, 10.f, 50.f);

			FLinearColor LinearColor = FLinearColor::Blue;
			if (Target == HoverVehicle->GetCurrentTarget())
			{
				LinearColor = FLinearColor::Red;
			}

			DrawRect(LinearColor, ScreenLocation.X - (Size / 2), ScreenLocation.Y - (Size / 2), Size, Size);
		}
	}
}