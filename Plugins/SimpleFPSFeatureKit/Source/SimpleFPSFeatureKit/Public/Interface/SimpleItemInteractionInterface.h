// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimpleItemInteractionInterface.generated.h"

class USkeletalMeshComponent;
// This class does not need to be modified.
UINTERFACE()
class USimpleItemInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLEFPSFEATUREKIT_API ISimpleItemInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//机制自定义，让用户自己决定使用哪个模型
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Item Interaction")
	USkeletalMeshComponent* GetCharacterMesh();
};
