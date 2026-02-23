// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimpleItemDefinition.generated.h"

/**
 * 
 */
class UAnimMontage;
UCLASS(BlueprintType, Blueprintable)
class SIMPLEFPSFEATUREKIT_API USimpleItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	//插槽名称，用于和ControlRig联动
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	FName ItemSlotName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	FTransform ItemRelativeTransform;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	TObjectPtr<UAnimMontage> StartTriggerAnimMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	TObjectPtr<UAnimMontage> EndTriggerAnimMontage;

public:
	USimpleItemDefinition();
};
