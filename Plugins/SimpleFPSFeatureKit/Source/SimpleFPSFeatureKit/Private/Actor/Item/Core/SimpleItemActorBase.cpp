// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Core/SimpleItemActorBase.h"

#include "SimpleFPSFeatureKitType.h"
#include "Components/SimpleItemInterComponent.h"
#include "Net/UnrealNetwork.h"


bool ASimpleItemActorBase::StartTrigger(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand)
{
	if (!IsStartTrigger(ItemInterComponent, bForceInHand))
	{
		UE_LOG(LogTemp, Error, TEXT("[SimpleItemActorBase] %s 正在交互 >w<"),*GetName())
		return false;
	}

	OnStartTrigger(ItemInterComponent, bForceInHand);
	
	return true;
}

bool ASimpleItemActorBase::EndTrigger(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack)
{
	if (!IsEndTrigger(ItemInterComponent, bIsPutPack))
	{
		UE_LOG(LogTemp, Error, TEXT("[SimpleItemActorBase] %s 正在交互 >w<"),*GetName())
		return false;
	}

	OnEndTrigger(ItemInterComponent, bIsPutPack);

	return true;
}

void ASimpleItemActorBase::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand)
{
	InteractingComponent = ItemInterComponent;
}

void ASimpleItemActorBase::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack)
{
	InteractingComponent.Reset();
}

void ASimpleItemActorBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASimpleItemActorBase, InteractingComponent);
}

ASimpleItemActorBase::ASimpleItemActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//开启网络同步
	bReplicates = true;
	//在场景加载时不创建蓝图副本
	bNetLoadOnClient = false;

	//给一个默认值
	ItemType = ESimpleKitItemType::ITEM_INVALID;
}

bool ASimpleItemActorBase::IsStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand)
{
	//当前没有和其他玩家交互，同时传入的组件有意义
	return InteractingComponent == nullptr && ItemInterComponent;
}

bool ASimpleItemActorBase::IsEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack)
{
	//当前的组件有意义，同时和传入的形参一致
	return InteractingComponent.IsValid() && InteractingComponent == ItemInterComponent;
}

// Called when the game starts or when spawned
void ASimpleItemActorBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASimpleItemActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
