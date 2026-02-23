// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleItemActorBase.generated.h"

enum class ESimpleKitItemType : uint8;
class USimpleItemInterComponent;

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorBase : public AActor
{
	GENERATED_BODY()

protected:
	//本物体的交互类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ItemActorBase")
	ESimpleKitItemType ItemType;
	//当前在和哪个组件做交互
	UPROPERTY(Replicated, BlueprintReadOnly, Category="ItemActorBase")
	TWeakObjectPtr<USimpleItemInterComponent> InteractingComponent;

public:
	//开始和结束交互
	UFUNCTION(BlueprintCallable, Category="ItemActorBase")
	bool StartTrigger(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand);

	UFUNCTION(BlueprintCallable, Category="ItemActorBase")
	bool EndTrigger(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack);

	//是否可以开始和结束交互
	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	bool IsStartTrigger(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand);

	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	bool IsEndTrigger(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack);

	//判断交互类型
	UFUNCTION(BlueprintPure, Category="ItemActorBase")
	ESimpleKitItemType GetItemType() const { return ItemType; }

protected:
	//开始和结束交互的响应事件
	//BlueprintNativeEvent，不清楚用户用的是蓝图函数还是C++
	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	void OnStartTrigger(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand);
	
	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	void OnEndTrigger(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Sets default values for this actor's properties
	ASimpleItemActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
