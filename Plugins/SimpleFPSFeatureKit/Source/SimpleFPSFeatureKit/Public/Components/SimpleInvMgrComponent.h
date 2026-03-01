// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ItemDefinition/SimpleItemPickableDefinition.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "SimpleInvMgrComponent.generated.h"

USTRUCT(BlueprintType)
struct FSimpleItemInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Inventory Entry")
	TSubclassOf<USimpleItemPickableDefinition> ItemDefinition;

	UPROPERTY(BlueprintReadOnly, Category="Inventory Entry")
	int32 ItemCounts;
};

inline bool operator==(const FSimpleItemInventoryEntry& SourceEntry, const FSimpleItemInventoryEntry& TargetEntry)
{
	return SourceEntry.ItemDefinition == TargetEntry.ItemDefinition && SourceEntry.ItemCounts == TargetEntry.ItemCounts;
}

USTRUCT(BlueprintType)
struct FSimpleItemInventoryList : public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY()

public:
	//下面都是网络同步，真正的操作在这边
	void SetInventorySize(const int32& NewInventorySize);
	//是否增删
	bool IsAddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDefinition,
	                FSimpleItemInventoryEntry*& ItemEntry);
	bool IsRemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDefinition, const int32& ItemCounts,
	                   FSimpleItemInventoryEntry*& ItemEntry);
	//增删
	int32 AddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDefinition, const int32& ItemCounts);
	int32 RemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDefinition, const int32& ItemCounts);
	//查和改
	FSimpleItemInventoryEntry* GetEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDefinition);

public:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FSimpleItemInventoryEntry, FSimpleItemInventoryList>(
			Entries, DeltaParams, *this);
	}

public:
	//网络同步相关
	void PreReplicatedRemove(const TArrayView<int32> RemoveIndices,int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddIndices,int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangeIndices,int32 FinalSize);

private:
	UPROPERTY(BlueprintReadOnly, Category="Inventory List", meta=(AllowPrivateAccess="true"))
	TArray<FSimpleItemInventoryEntry> Entries;

	UPROPERTY(BlueprintReadOnly, Category="Inventory List", meta=(AllowPrivateAccess="true"))
	int32 InventorySize = 1;
};

UCLASS(ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimpleInvMgrComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimpleInvMgrComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
