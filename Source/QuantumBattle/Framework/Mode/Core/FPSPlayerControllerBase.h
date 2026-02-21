// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMBATTLE_API AFPSPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
