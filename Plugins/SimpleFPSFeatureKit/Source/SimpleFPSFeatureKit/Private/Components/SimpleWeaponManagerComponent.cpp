// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleWeaponManagerComponent.h"


// Sets default values for this component's properties
USimpleWeaponManagerComponent::USimpleWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//默认支持网络同步
	SetIsReplicatedByDefault(true);
}

void USimpleWeaponManagerComponent::InitWeaponManager(int32 InSlotSize)
{
	check(GetOwner() && GetOwner()->HasAuthority() && InSlotSize > 0)

	if (Weapons.Num() > 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[USimpleWeaponManagerComponent::InitWeaponManager]武器管理类还没有初始化成功哦>w<"))
	}
	Weapons.SetNum(InSlotSize);
}

ASimpleItemActorWeapon* USimpleWeaponManagerComponent::GetWeaponInSlot(int32 WeaponSlot)
{
	return nullptr;
}

bool USimpleWeaponManagerComponent::AddWeapon(ASimpleItemActorWeapon* NewWeapon)
{
	return false;
}

bool USimpleWeaponManagerComponent::RemoveWeapon(int32 InRemoveSlot)
{
	return false;
}

bool USimpleWeaponManagerComponent::EquipWeapon(int32 InSlot)
{
	return false;
}

bool USimpleWeaponManagerComponent::UnequipWeapon()
{
	return false;
}

bool USimpleWeaponManagerComponent::ReplaceWeapon(ASimpleItemActorWeapon* NewWeapon)
{
	return false;
}

void USimpleWeaponManagerComponent::OnRep_EquipSlot(const int32& OldEquipSlot)
{
}

void USimpleWeaponManagerComponent::OnRep_EquippingWeapon(const TWeakObjectPtr<ASimpleItemActorWeapon>& OldWeapon)
{
}

bool USimpleWeaponManagerComponent::GetWeaponSlot(const ASimpleItemActorWeapon* InWeapon, int32& OutSlot)
{
	return false;
}

bool USimpleWeaponManagerComponent::OnWeaponEquipped(ASimpleItemActorWeapon* EquippedWeapon)
{
	return false;
}

bool USimpleWeaponManagerComponent::OnWeaponUnequipped(ASimpleItemActorWeapon* UnequippedWeapon)
{
	return false;
}

void USimpleWeaponManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts
void USimpleWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
