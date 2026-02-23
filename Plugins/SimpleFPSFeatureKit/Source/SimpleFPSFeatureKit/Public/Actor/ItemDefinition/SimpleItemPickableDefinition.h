// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleItemDefinition.h"
#include "SimpleItemPickableDefinition.generated.h"

/**
 * 
 */
class ASimpleItemActorInventory;
UCLASS(BlueprintType, Blueprintable)
class SIMPLEFPSFEATUREKIT_API USimpleItemPickableDefinition : public USimpleItemDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Defaults")
	float ThrowItemSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Defaults")
	int32 ItemMaxCounts;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Defaults")
	bool bAllowInHand;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Defaults")
	TSubclassOf<ASimpleItemActorInventory> ItemClass;

public:
	USimpleItemPickableDefinition();
};
