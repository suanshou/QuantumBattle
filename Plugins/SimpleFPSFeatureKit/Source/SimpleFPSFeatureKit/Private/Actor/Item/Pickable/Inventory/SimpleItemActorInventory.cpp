// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Pickable/Inventory/SimpleItemActorInventory.h"

#include "SimpleFPSFeatureKitType.h"
#include "Components/SimpleInvMgrComponent.h"
#include "Components/SimpleItemInterComponent.h"


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
	if (bForceInHand)
	{
		return Super::IsStartTrigger_Implementation(ItemInterComponent, bForceInHand);
	}
	else
	{
		if (!Super::IsStartTrigger_Implementation(ItemInterComponent, bForceInHand))
		{
			return false;
		}

		if (ItemInterComponent->GetOwner() == nullptr)
		{
			return false;
		}

		USimpleInvMgrComponent* InventoryComponent = ItemInterComponent->GetOwner()->FindComponentByClass<
			USimpleInvMgrComponent>();

		if (InventoryComponent == nullptr)
		{
			return false;
		}

		return InventoryComponent->IsAddItemToInventory(ItemDefinition);
	}
}

void ASimpleItemActorInventory::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                              bool bForceInHand)
{
	if (bForceInHand)
	{
		Super::OnStartTrigger_Implementation(ItemInterComponent, bForceInHand);
	}
	else
	{
		ASimpleItemActorBase::OnStartTrigger_Implementation(ItemInterComponent, bForceInHand);

		ItemCounts -= PickUpItemToInventory();
		if (ItemCounts <= 0)
		{
			Destroy();
		}
	}
}

void ASimpleItemActorInventory::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                            bool bIsPutPack)
{
	if (bIsPutPack)
	{
		check(PickUpItemToInventory() == ItemCounts)

		Destroy();
	}
	else
	{
		Super::OnEndTrigger_Implementation(ItemInterComponent, bIsPutPack);
	}
}

int32 ASimpleItemActorInventory::PickUpItemToInventory()
{
	if (InteractingComponent == nullptr || InteractingComponent->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("[ASimpleItemActorInventory::PickUpItemToInventory]无效的交互物或交互物Owner，不能添加物体到列表>w<"))
		return 0;
	}

	USimpleInvMgrComponent* InventoryComponent = InteractingComponent->GetOwner()->FindComponentByClass<
		USimpleInvMgrComponent>();

	if (InventoryComponent == nullptr)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("[ASimpleItemActorInventory::PickUpItemToInventory]Owner没有InventoryComponent，不能添加物体到列表>w<"))
		return 0;
	}

	//执行加载流程
	return InventoryComponent->AddItemToInventory(ItemDefinition, ItemCounts);
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
