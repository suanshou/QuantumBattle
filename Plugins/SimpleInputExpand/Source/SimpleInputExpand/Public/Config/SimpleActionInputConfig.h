// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SimpleActionInputConfig.generated.h"

/**
 * 
 */
class UInputAction;
struct FGameplayTag;

USTRUCT(BlueprintType)
struct SIMPLEINPUTEXPAND_API FSimpleInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;
};

UCLASS(BlueprintType, Const)
class SIMPLEINPUTEXPAND_API USimpleActionInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	//用户可以配置标签对应的动作
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = "InputConfig")
	TArray<FSimpleInputAction> InputActions;

public:
	UFUNCTION(BlueprintCallable, Category = "InputConfig")
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;
};
