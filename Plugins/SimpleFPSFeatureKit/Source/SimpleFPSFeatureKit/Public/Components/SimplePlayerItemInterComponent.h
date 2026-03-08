// Fill out your copyright notice in the Description page of Project Settings.
//在此处扫描到交互主体

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SimpleItemInterComponent.h"
#include "Animation/AnimInstanceProxy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SimplePlayerItemInterComponent.generated.h"

struct FInputActionValue;
class ASimpleItemActorWeapon;
class USimpleWeaponManagerComponent;

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
	//拾取和丢弃
	virtual void Trigger(const FInputActionValue& Value);
	virtual void Throw(const FInputActionValue& Value);

public:
	//@TODO:在找到最近物体后需要材质边缘高亮
	void CheckItemAroundPlayer();

public:
	//识别到交互物是武器的时候拾取武器
	void PickupNewWeapon(ASimpleItemActorWeapon* InNewWeapon, bool bForceInHand);

	USimpleWeaponManagerComponent* GetWeaponManager() const;

protected:
	//开始交互
	//对交互物品做分类，分开做交互逻辑
	virtual void
	OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem, bool bForceInHand) override;
	//停止交互
	virtual void
	OnInteractingItemTriggerEnd_Implementation(ASimpleItemActorBase* InInteractingItem, bool bIsPutPack) override;
};
