// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/FPSCharacterBase.h"
#include "Interface/SimpleInputInterface.h"
#include "FPSPlayer.generated.h"

UCLASS(config=Game)
class QUANTUMBATTLE_API AFPSPlayer : public AFPSCharacterBase, public ISimpleInputInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void K2_OnActionInputTag_Implementation(
		ETriggerEvent InEvent,
		const FInputActionValue& InValue,
		FGameplayTag InputTag) override;
};
