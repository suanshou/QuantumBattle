// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleModularCharacter.h"
#include "FPSCharacterBase.generated.h"

//只管攻击，伤害和接受伤害
UCLASS(config=Game)
class QUANTUMBATTLE_API AFPSCharacterBase : public ASimpleModularCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
