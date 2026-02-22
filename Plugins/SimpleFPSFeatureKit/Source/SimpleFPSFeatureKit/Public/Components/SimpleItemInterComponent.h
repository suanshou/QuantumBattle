// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleItemInterComponent.generated.h"

class ASimpleItemActorBase;
class FLifetimeProperty;

UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimpleItemInterComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category="Config")
	TWeakObjectPtr<ASimpleItemActorBase> SelectingItem;
	UPROPERTY(ReplicatedUsing="OnRep_InteractingItem", BlueprintReadOnly, Category="Config")
	TWeakObjectPtr<ASimpleItemActorBase> InteractingItem;

public:
	// Sets default values for this component's properties
	USimpleItemInterComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category="ItemInteractionComponent")
	ASimpleItemActorBase* GetInteractingItem() const;
	//看源码只要从这两个函数开始看
	UFUNCTION(BlueprintCallable, Category="ItemInteractionComponent")
	void StartTriggerSelectingItem();
	UFUNCTION(BlueprintCallable, Category="ItemInteractionComponent")
	void EndTriggerInteractingItem();

public:
	//决定是否选择
	//BlueprintNativeEvent虚接口在GENERATED_BODY()实现
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsStartInteraction() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsEndInteraction() const;
	//决定是否交互
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsStartTriggerSelectingItem() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsEndTriggerInteractingItem() const;

protected:
	//网络同步
	//不可靠常用于UDP，方便高频发送信息
	UFUNCTION(Server, Unreliable)
	void StartTriggerSelectingItemOnServer(ASimpleItemActorBase* InSelectingItem);
	UFUNCTION(Server, Unreliable)
	void EndTriggerInteractingItemOnServer(bool bIsPutPack);
	//开始和结束交互
	UFUNCTION(BlueprintNativeEvent, Category="ItemInteractionComponent")
	void OnSelectingItemTriggerStart(ASimpleItemActorBase* InSelectingItem, bool bForceInHand);
	UFUNCTION(BlueprintNativeEvent, Category="ItemInteractionComponent")
	void OnInteractingItemTriggerEnd(ASimpleItemActorBase* InInteractingItem, bool bIsPutPack);

public:
	//网络同步支持，方便在之后和GAS联动
	void ServerTriggerItem(ASimpleItemActorBase* NewTriggerItem, bool bForceInHand = false);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION()
	void OnRep_InteractingItem();
};
