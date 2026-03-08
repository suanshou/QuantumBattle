// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleInvMgrComponent.h"

#include "Actor/Item/Pickable/Inventory/SimpleItemActorInventory.h"
#include "Components/SimplePlayerItemInterComponent.h"
#include "Net/UnrealNetwork.h"


void FSimpleItemInventoryList::SetInventorySize(const int32& NewInventorySize)
{
	if (NewInventorySize < Entries.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("[FSimpleItemInventoryList::SetInventorySize]新的库存物无效，无法修改值>w<"))
		return;
	}

	InventorySize = NewInventorySize;
}

bool FSimpleItemInventoryList::IsAddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDefinition,
                                          FSimpleItemInventoryEntry*& ItemEntry)
{
	for (auto& TmpEntry : Entries)
	{
		if (TmpEntry.ItemDefinition == NewItemDefinition)
		{
			//判断物品能不能堆叠
			if (TmpEntry.ItemCounts < NewItemDefinition.GetDefaultObject()->ItemMaxCounts)
			{
				ItemEntry = &TmpEntry;
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[FSimpleItemInventoryList::IsAddEntry]不能添加物品，因为物品数量已经超过了最大单元容量哦>w<"))
				return false;
			}
		}
	}

	//超过最大物品数就创建新的单元格
	return Entries.Num() < InventorySize;
}

bool FSimpleItemInventoryList::IsRemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDefinition,
                                             const int32& ItemCounts, FSimpleItemInventoryEntry*& ItemEntry)
{
	for (auto& TmpEntry : Entries)
	{
		if (TmpEntry.ItemDefinition == InItemDefinition)
		{
			if (TmpEntry.ItemCounts < ItemCounts)
			{
				UE_LOG(LogTemp, Error, TEXT("[FSimpleItemInventoryList::IsRemoveEntry]不能移除物品，因为物品数量已经超过了最大单元容量哦>w<"))
				return false;
			}
			else
			{
				ItemEntry = &TmpEntry;
				return true;
			}
		}
	}

	return false;
}

int32 FSimpleItemInventoryList::AddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDefinition,
                                         const int32& ItemCounts)
{
	int32 RealAddCounts = 0;
	FSimpleItemInventoryEntry* TargetItemEntry = nullptr;

	if (IsAddEntry(NewItemDefinition, TargetItemEntry))
	{
		int32 LastCounts = 0;

		if (TargetItemEntry)
		{
			LastCounts = TargetItemEntry->ItemCounts;
			TargetItemEntry->ItemCounts += ItemCounts;
		}
		else
		{
			//创建对象
			TargetItemEntry = &Entries.AddDefaulted_GetRef();
			TargetItemEntry->ItemDefinition = NewItemDefinition;
			TargetItemEntry->ItemCounts = ItemCounts;
		}

		//最大数限制
		if (TargetItemEntry->ItemCounts > NewItemDefinition.GetDefaultObject()->ItemMaxCounts)
		{
			TargetItemEntry->ItemCounts = NewItemDefinition.GetDefaultObject()->ItemMaxCounts;
		}

		RealAddCounts = TargetItemEntry->ItemCounts - LastCounts;

		//网络同步标记
		//改动基本在服务器进行，这样标记可以只更改一部分，节省性能
		MarkItemDirty(*TargetItemEntry);
	}

	return RealAddCounts;
}

int32 FSimpleItemInventoryList::RemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDefinition,
                                            const int32& ItemCounts)
{
	int32 RealRemoveCounts = 0;
	FSimpleItemInventoryEntry* TargetItemEntry = nullptr;

	if (IsRemoveEntry(InItemDefinition, ItemCounts, TargetItemEntry))
	{
		TargetItemEntry->ItemCounts -= ItemCounts;

		if (TargetItemEntry->ItemCounts == 0)
		{
			Entries.RemoveSingle(*TargetItemEntry);

			//设为脏，后续收集信息的时候自动同步
			MarkArrayDirty();
		}
		else
		{
			MarkItemDirty(*TargetItemEntry);
		}

		RealRemoveCounts = ItemCounts;
	}

	return RealRemoveCounts;
}

FSimpleItemInventoryEntry* FSimpleItemInventoryList::GetEntry(
	const TSubclassOf<USimpleItemPickableDefinition>& InItemDefinition)
{
	//遍历
	return Entries.FindByPredicate(
		[InItemDefinition](const FSimpleItemInventoryEntry& TmpEntry)-> bool
		{
			return TmpEntry.ItemDefinition == InItemDefinition;
		});
}

void FSimpleItemInventoryList::PreReplicatedRemove(const TArrayView<int32> RemoveIndices, int32 FinalSize)
{
}

void FSimpleItemInventoryList::PostReplicatedAdd(const TArrayView<int32> AddIndices, int32 FinalSize)
{
}

void FSimpleItemInventoryList::PostReplicatedChange(const TArrayView<int32> ChangeIndices, int32 FinalSize)
{
}

USimplePlayerItemInterComponent* USimpleInvMgrComponent::GetItemInteractionComponent()
{
	check(GetOwner())

	return GetOwner()->FindComponentByClass<USimplePlayerItemInterComponent>();
}

bool USimpleInvMgrComponent::GetItemEntry(TSubclassOf<USimpleItemPickableDefinition> ItemDefinition,
                                          FSimpleItemInventoryEntry& TargetEntry)
{
	TargetEntry = FSimpleItemInventoryEntry();
	
	if (FSimpleItemInventoryEntry* ItemEntry = InventoryList.GetEntry(ItemDefinition))
	{
		TargetEntry = *ItemEntry;
		return true;
	}

	return false;
}

bool USimpleInvMgrComponent::IsAddItemToInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDefinition)
{
	FSimpleItemInventoryEntry* TempEntry = nullptr;
	
	return InventoryList.IsAddEntry(ItemDefinition, TempEntry);
}

bool USimpleInvMgrComponent::IsRemoveFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDefinition,
                                                   const int32& ItemCounts)
{
	FSimpleItemInventoryEntry* TempEntry = nullptr;

	return InventoryList.IsRemoveEntry(ItemDefinition, ItemCounts, TempEntry);
}

int32 USimpleInvMgrComponent::AddItemToInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDefinition,
                                                 const int32& ItemCounts)
{
	check(GetOwner() && GetOwner()->HasAuthority())

	return InventoryList.AddEntry(ItemDefinition, ItemCounts);
}

int32 USimpleInvMgrComponent::RemoveItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDefinition,
                                                      const int32& ItemCounts)
{
	check(GetOwner() && GetOwner()->HasAuthority())

	//return InventoryList.RemoveEntry(ItemDefinition, ItemCounts);
	return false;
}

void USimpleInvMgrComponent::DiscardItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDefinition,
                                                      const int32& ItemCounts)
{
	check(GetOwner() && GetOwner()->HasAuthority() && GetWorld())

	if (ItemDefinition)
	{
		const USimpleItemPickableDefinition* TmpItemDef = ItemDefinition.GetDefaultObject();
		if (TmpItemDef->ItemClass)
		{
			if (InventoryList.RemoveEntry(ItemDefinition, ItemCounts))
			{
				FTransform SpawnTransform(
					GetOwner()->GetActorRotation(),
					GetOwner()->GetActorLocation(),
					FVector::OneVector);

				FActorSpawnParameters SpawnParameters;
				SpawnParameters.SpawnCollisionHandlingOverride =
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				//生成物体后更新数据
				ASimpleItemActorInventory* DiscardItem = GetWorld()->SpawnActor<ASimpleItemActorInventory>(
					TmpItemDef->ItemClass,
					SpawnTransform,
					SpawnParameters);

				if (DiscardItem)
				{
					DiscardItem->SetItemCounts(ItemCounts);
				}
			}
		}
	}
}

void USimpleInvMgrComponent::TakeOutItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDefinition)
{
	check(GetOwner() && GetOwner()->HasAuthority() && GetWorld())

	if (ItemDefinition)
	{
		const USimpleItemPickableDefinition* TmpItemDef = ItemDefinition.GetDefaultObject();
		USimplePlayerItemInterComponent* IC_Player = GetItemInteractionComponent();

		if (IC_Player && TmpItemDef->bAllowInHand && TmpItemDef->ItemClass)
		{
			if (InventoryList.RemoveEntry(ItemDefinition, 1))
			{
				FTransform SpawnTransform(
					GetOwner()->GetActorRotation(),
					GetOwner()->GetActorLocation(),
					FVector::OneVector);

				ASimpleItemActorInventory* TakeOutItem = GetWorld()->SpawnActor<ASimpleItemActorInventory>(
					TmpItemDef->ItemClass,
					SpawnTransform);

				if (TakeOutItem)
				{
					TakeOutItem->SetItemCounts(1);

					//切枪相关的操作也放在这里
					IC_Player->ServerTriggerItem(TakeOutItem, true);
				}
			}
		}
	}
}

void USimpleInvMgrComponent::DiscardItemFromInventoryOnServer_Implementation(
	TSubclassOf<USimpleItemPickableDefinition> ItemDefinition, int32 ItemCounts)
{
	DiscardItemFromInventory(ItemDefinition, ItemCounts);
}

void USimpleInvMgrComponent::TakeOutItemFromInventoryOnServer_Implementation(
	TSubclassOf<USimpleItemPickableDefinition> ItemDefinition)
{
	TakeOutItemFromInventory(ItemDefinition);
}

// Sets default values for this component's properties
USimpleInvMgrComponent::USimpleInvMgrComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//默认支持网络同步
	SetIsReplicatedByDefault(true);
}

void USimpleInvMgrComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, InventoryList);
}


// Called when the game starts
void USimpleInvMgrComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleInvMgrComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
