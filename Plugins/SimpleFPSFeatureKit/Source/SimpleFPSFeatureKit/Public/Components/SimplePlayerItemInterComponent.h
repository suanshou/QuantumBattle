// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SimpleItemInterComponent.h"
#include "Animation/AnimInstanceProxy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SimplePlayerItemInterComponent.generated.h"

struct FInputActionValue;

UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimplePlayerItemInterComponent : public USimpleItemInterComponent
{
	GENERATED_BODY()

protected:
	//碰撞检测通道
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TEnumAsByte<ECollisionChannel> CheckCollisionType;
	//Debug类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTraceMode;
	//检测范围
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Config", meta=(ClampMin = 0.f))
	float SelectItemActorRange;
	//检测相对于角色的偏移
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config")
	FVector SphereCenterOffset;
	//检测的帧间隔
	UPROPERTY()
	float TriggerInterval;

public:
	//记录附近的物品
	UPROPERTY(BlueprintReadOnly, Category="Config")
	TArray<ASimpleItemActorBase*> NearbyItems;

public:
	// Sets default values for this component's properties
	USimplePlayerItemInterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void Trigger(const FInputActionValue& Value);
	//@TODO:丢掉的输入需要按下启动，松开丢出，同时需要取消的逻辑
	virtual void Throw(const FInputActionValue& Value);

public:
	//@TODO:在找到最近物体后需要材质边缘高亮
	void CheckItemAroundPlayer();

protected:
	virtual void
	OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem, bool bForceInHand) override;
	virtual void
	OnInteractingItemTriggerEnd_Implementation(ASimpleItemActorBase* InInteractingItem, bool bIsPutPack) override;
};
