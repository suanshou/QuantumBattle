// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Pickable/Core/SimpleItemActorPickable.h"

#include "SimpleFPSFeatureKitType.h"
#include "Actor/ItemDefinition/SimpleItemPickableDefinition.h"
#include "Components/SimpleItemInterComponent.h"
#include "GameFramework/Character.h"
#include "Interface/SimpleItemInteractionInterface.h"


USimpleItemPickableDefinition* ASimpleItemActorPickable::GetItemDefinition() const
{
	return ItemDefinition.GetDefaultObject();
}

void ASimpleItemActorPickable::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                             bool bForceInHand)
{
	Super::OnStartTrigger_Implementation(ItemInterComponent, bForceInHand);

	USkeletalMeshComponent* CharacterMesh = nullptr;
	if (ISimpleItemInteractionInterface* ItemInterface = Cast<ISimpleItemInteractionInterface>(ItemInterComponent->GetOwner()))
	{
		//接口必须使用Execute，这样才能知道是蓝图还是C++使用
		CharacterMesh = ItemInterface->Execute_GetCharacterMesh(ItemInterComponent->GetOwner());
	}
	else
	{
		ACharacter* CharacterOwner = Cast<ACharacter>(ItemInterComponent->GetOwner());
		if (CharacterOwner)
		{
			CharacterMesh = CharacterOwner->GetMesh();
		}
	}

	if (ItemDefinition)
	{
		//播放动画
		if (ItemDefinition.GetDefaultObject()->StartTriggerAnimMontage)
		{
			ItemInterComponent->PlayMontageNetMulticast(ItemDefinition.GetDefaultObject()->StartTriggerAnimMontage);
		}

		AttachToComponent(
			CharacterMesh,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			ItemDefinition.GetDefaultObject()->ItemSlotName);

		SetActorRelativeTransform(ItemDefinition.GetDefaultObject()->ItemRelativeTransform);
	}
}

void ASimpleItemActorPickable::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                           bool bIsPutPack)
{
	Super::OnEndTrigger_Implementation(ItemInterComponent, bIsPutPack);
}

void ASimpleItemActorPickable::PickupItem_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                         UPrimitiveComponent* InItemMesh)
{
	if (InItemMesh)
	{
		//取消物理模拟
		InItemMesh->SetSimulatePhysics(false);
		//相对位置清零
		InItemMesh->SetRelativeLocation(FVector::ZeroVector);
		//取消碰撞
		InItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//取消移动的网络同步
		//SetReplicatingMovement(false);
	}
}

void ASimpleItemActorPickable::ThrowItem_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                        UPrimitiveComponent* InItemMesh, bool bIsPutPack)
{
	if (InItemMesh && ItemDefinition)
	{
		//开启移动的网络同步
		//SetReplicatingMovement(true);

		InItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		InItemMesh->SetSimulatePhysics(true);

		if (!bIsPutPack)
		{
			if (ItemInterComponent && ItemInterComponent->GetOwner())
			{
				InItemMesh->AddImpulse(
					ItemInterComponent->GetOwner()->GetActorForwardVector() *
					ItemDefinition.GetDefaultObject()->ThrowItemSpeed,
					NAME_None, true);
			}
		}
	}
}

// Sets default values
ASimpleItemActorPickable::ASimpleItemActorPickable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemType = ESimpleKitItemType::ITEM_PICKABLE;
	PickableType = ESimpleKitItemPickableType::ITEM_NORMAL;
}

// Called when the game starts or when spawned
void ASimpleItemActorPickable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASimpleItemActorPickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
