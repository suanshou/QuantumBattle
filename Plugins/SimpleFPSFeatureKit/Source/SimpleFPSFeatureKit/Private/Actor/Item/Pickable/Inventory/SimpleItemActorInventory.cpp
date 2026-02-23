// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Pickable/Inventory/SimpleItemActorInventory.h"

#include "SimpleFPSFeatureKitType.h"


void ASimpleItemActorInventory::SetItemCounts(int32 InItemCounts)
{
	//数字赋值只在服务器上运行
	if (HasAuthority())
	{
		ItemCounts = InItemCounts;
	}
}

bool ASimpleItemActorInventory::IsStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
	bool bForceInHand)
{
	return Super::IsStartTrigger_Implementation(ItemInterComponent, bForceInHand);
}

void ASimpleItemActorInventory::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
	bool bForceInHand)
{
	Super::OnStartTrigger_Implementation(ItemInterComponent, bForceInHand);
}

void ASimpleItemActorInventory::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
	bool bIsPutPack)
{
	Super::OnEndTrigger_Implementation(ItemInterComponent, bIsPutPack);
}

int32 ASimpleItemActorInventory::PickUpItemToInventory()
{
	return 0;
}

ASimpleItemActorInventory::ASimpleItemActorInventory()
{
	PickableType = ESimpleKitItemPickableType::ITEM_INVENTORY;
	InventoryType = ESimpleItemInventoryType::ITEM_NORMAL;
}

// Called when the game starts or when spawned
void ASimpleItemActorInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimpleItemActorInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

