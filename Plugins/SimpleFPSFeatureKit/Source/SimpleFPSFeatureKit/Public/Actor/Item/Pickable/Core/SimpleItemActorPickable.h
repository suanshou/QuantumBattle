// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Core/SimpleItemActorBase.h"
#include "SimpleItemActorPickable.generated.h"

enum class ESimpleKitItemPickableType : uint8;
class USimpleItemPickableDefinition;

UCLASS()
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorPickable : public ASimpleItemActorBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ItemActorPickable")
	ESimpleKitItemPickableType PickableType;
	
	UPROPERTY(Editanywhere, BlueprintReadOnly, Category="ItemActorPickable")
	TSubclassOf<USimpleItemPickableDefinition> ItemDefinition;

public:
	UFUNCTION(BlueprintPure, Category="TPItemActor_Pickable")
	ESimpleKitItemPickableType GetPickableType() const { return PickableType; }

	UFUNCTION(BlueprintPure, Category="TPItemActor_Pickable")
	USimpleItemPickableDefinition* GetItemDefinition() const;

protected:
	virtual void OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand) override;
	virtual void OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack) override;

protected:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Pickable")
	void PickupItem(USimpleItemInterComponent* ItemInterComponent,UPrimitiveComponent* InItemMesh);

	//bIsPutPack表示是否在背包内
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Pickable")
	void ThrowItem(USimpleItemInterComponent* ItemInterComponent,UPrimitiveComponent* InItemMesh,bool bIsPutPack);
	
public:
	// Sets default values for this actor's properties
	ASimpleItemActorPickable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
