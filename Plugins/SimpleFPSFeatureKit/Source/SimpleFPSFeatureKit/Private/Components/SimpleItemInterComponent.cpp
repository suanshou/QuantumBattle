// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleItemInterComponent.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USimpleItemInterComponent::USimpleItemInterComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void USimpleItemInterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
		//支持网络同步
	SetIsReplicated(true);
}


// Called every frame
void USimpleItemInterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ASimpleItemActorBase* USimpleItemInterComponent::GetInteractingItem() const
{
	return InteractingItem.Get();
}

void USimpleItemInterComponent::StartTriggerSelectingItem()
{
	if (!IsStartTriggerSelectingItem())
	{
		UE_LOG(LogTemp, Error, TEXT("[StartTriggerSelectingItem]不能在本地Trigger >w<"))
		return;
	}

	StartTriggerSelectingItemOnServer(SelectingItem.Get());
}

void USimpleItemInterComponent::EndTriggerInteractingItem()
{
	if (!IsEndTriggerInteractingItem())
	{
		UE_LOG(LogTemp, Error, TEXT("[EndTriggerSelectingItem]不能在本地Trigger >w<"))
		return;
	}

	EndTriggerInteractingItemOnServer(false);
}

void USimpleItemInterComponent::PlayMontageNetMulticast_Implementation(
	UAnimMontage* InMontage,
	float InPlayRate,
	FName StartSelectionName)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner()))
	{
		PlayerCharacter->PlayAnimMontage(InMontage, InPlayRate, StartSelectionName);
	}
}

void USimpleItemInterComponent::ServerTriggerItem(ASimpleItemActorBase* NewTriggerItem, bool bForceInHand)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		//泛化内容
		OnSelectingItemTriggerStart(NewTriggerItem, bForceInHand);
	}
}

void USimpleItemInterComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USimpleItemInterComponent,InteractingItem);
}

void USimpleItemInterComponent::OnRep_InteractingItem()
{
}

bool USimpleItemInterComponent::IsStartInteraction_Implementation() const
{
	return true;
}

bool USimpleItemInterComponent::IsEndInteraction_Implementation() const
{
	return true;
}

bool USimpleItemInterComponent::IsStartTriggerSelectingItem_Implementation() const
{
	if (!SelectingItem.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[IsStartTriggerInteractingItem]组件上没有SelectingItem >w<"))
		return false;
	}

	return true;
}

bool USimpleItemInterComponent::IsEndTriggerInteractingItem_Implementation() const
{
	if (InteractingItem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[IsEndTriggerInteractingItem]组件上没有InteractingItem >w<"))
		return false;
	}

	return true;
}

void USimpleItemInterComponent::StartTriggerSelectingItemOnServer_Implementation(ASimpleItemActorBase* InSelectingItem)
{
	OnSelectingItemTriggerStart(InSelectingItem, false);
}

void USimpleItemInterComponent::EndTriggerInteractingItemOnServer_Implementation(bool bIsPutPack)
{
	OnInteractingItemTriggerEnd(InteractingItem.Get(),bIsPutPack);
}

void USimpleItemInterComponent::OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem,
	bool bForceInHand)
{
}

void USimpleItemInterComponent::OnInteractingItemTriggerEnd_Implementation(ASimpleItemActorBase* InInteractingItem,
	bool bIsPutPack)
{
}