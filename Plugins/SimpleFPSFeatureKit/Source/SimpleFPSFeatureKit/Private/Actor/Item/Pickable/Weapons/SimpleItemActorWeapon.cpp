// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Pickable/Weapons/SimpleItemActorWeapon.h"

#include "SimpleFPSFeatureKitType.h"
#include "Actor/Item/Pickable/Weapons/SimpleWeaponInstance.h"
#include "Actor/ItemDefinition/SimpleItemPickableDefinition.h"
#include "Components/SimpleItemInterComponent.h"
#include "Interface/SimpleItemInteractionInterface.h"
#include "Net/UnrealNetwork.h"


void ASimpleItemActorWeapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, WeaponInstance);
}

// Sets default values
ASimpleItemActorWeapon::ASimpleItemActorWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickableType = ESimpleKitItemPickableType::ITEM_WEAPON;
	//开启网络同步注册
	bReplicateUsingRegisteredSubObjectList = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	//碰撞预设
	WeaponMesh->SetCollisionProfileName(FName(TEXT("ItemPickable")));
	WeaponMesh->SetIsReplicated(true);
};

// Called when the game starts or when spawned
void ASimpleItemActorWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && WeaponInstance == nullptr)
	{
		if (WeaponInstanceClass)
		{
			WeaponInstance = NewObject<USimpleWeaponInstance>(this, WeaponInstanceClass);

			if (WeaponInstance)
			{
				WeaponInstance->InitInstance();
				AddReplicatedSubObject(WeaponInstance);
			}
		}
	}
}

void ASimpleItemActorWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
	{
		RemoveReplicatedSubObject(WeaponInstance);
	}

	Super::EndPlay(EndPlayReason);
}

void ASimpleItemActorWeapon::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                           bool bForceInHand)
{
	Super::OnStartTrigger_Implementation(ItemInterComponent, bForceInHand);

	if (WeaponInstance == nullptr)
	{
		return;
	}
}

void ASimpleItemActorWeapon::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack)
{
	Super::OnEndTrigger_Implementation(ItemInterComponent, bIsPutPack);

	if (WeaponInstance)
	{
		
	}
}

void ASimpleItemActorWeapon::EquipToSlot()
{
	if (InteractingComponent.IsValid())
	{
		//InteractingComponent->GetOwner()找到用户希望把模型放到哪里
		if (ISimpleItemInteractionInterface* ItemInterface = Cast<ISimpleItemInteractionInterface>(
			InteractingComponent->GetOwner()))
		{
			if (USkeletalMeshComponent* CharacterMesh = ItemInterface->Execute_GetCharacterMesh(
				InteractingComponent->GetOwner()))
			{
				PickupItem(InteractingComponent.Get(), CharacterMesh);
			}
		}
	}
}

void ASimpleItemActorWeapon::EquipToHand()
{
	if (InteractingComponent.IsValid())
	{
		if (ISimpleItemInteractionInterface* ItemInterface = Cast<ISimpleItemInteractionInterface>(
			InteractingComponent->GetOwner()))
		{
			if (USkeletalMeshComponent* CharacterMesh = ItemInterface->Execute_GetCharacterMesh(
				InteractingComponent->GetOwner()))
			{
				if (ItemDefinition)
				{
					PickupItem(InteractingComponent.Get(), CharacterMesh);

					AttachToComponent(CharacterMesh,
					                  FAttachmentTransformRules::SnapToTargetNotIncludingScale,
					                  ItemDefinition.GetDefaultObject()->ItemSlotName);
					SetActorRelativeTransform(ItemDefinition.GetDefaultObject()->ItemRelativeTransform);
				}
			}
		}
	}
}

// Called every frame
void ASimpleItemActorWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
