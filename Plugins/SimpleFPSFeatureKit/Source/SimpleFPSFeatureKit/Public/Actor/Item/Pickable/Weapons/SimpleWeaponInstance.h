// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimpleWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEFPSFEATUREKIT_API USimpleWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	USimpleWeaponInstance();
	//在实例化时的例如添加弹药等等逻辑
	void InitInstance();
};
