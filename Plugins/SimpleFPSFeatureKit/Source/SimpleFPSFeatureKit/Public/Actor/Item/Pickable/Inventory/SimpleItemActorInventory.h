// Fill out your copyright notice in the Description page of Project Settings.
//后续需要一个库存管理类，也就是背包，和这个库存泛化类联动

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Pickable/Core/SimpleItemActorPickable.h"
#include "SimpleItemActorInventory.generated.h"

enum class ESimpleItemInventoryType : uint8;

UCLASS()
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorInventory : public ASimpleItemActorPickable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	ESimpleItemInventoryType InventoryType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 ItemCounts;

public:
	UFUNCTION(BlueprintCallable, Category="Inventory")
	ESimpleItemInventoryType GetInventoryType() const { return InventoryType; }

	UFUNCTION(BlueprintCallable, Category="TPItemActor_Inventory")
	void SetItemCounts(int32 InItemCounts);

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetItemCounts() const { return ItemCounts; }

protected:
	virtual bool IsStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand) override;
	virtual void OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand) override;
	virtual void OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack) override;
	
protected:
	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 PickUpItemToInventory();

public:
	// Sets default values for this actor's properties
	ASimpleItemActorInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
