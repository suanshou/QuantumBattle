// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimplePlayerItemInterComponent.h"

#include "SimpleFPSFeatureKitType.h"
#include "Actor/Item/Core/SimpleItemActorBase.h"
#include "Actor/Item/Pickable/Core/SimpleItemActorPickable.h"
#include "Actor/Item/Pickable/Inventory/SimpleItemActorInventory.h"
#include "Actor/Item/Pickable/Weapons/SimpleItemActorWeapon.h"
#include "Actor/Item/Pickable/Weapons/SimpleWeaponInstance.h"
#include "Components/SimpleWeaponManagerComponent.h"


// Sets default values for this component's properties
USimplePlayerItemInterComponent::USimplePlayerItemInterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SelectItemActorRange = 150.f;
	SphereCenterOffset = FVector::ZeroVector;
	TriggerInterval = 0.f;
	//暂时定一个值，之后再改
	CheckCollisionType = ECollisionChannel::ECC_GameTraceChannel1;
}


// Called when the game starts
void USimplePlayerItemInterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimplePlayerItemInterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//客户端检测，所见即所得
	if (GetOwner() && GetOwner()->HasLocalNetOwner())
	{
		CheckItemAroundPlayer();

		TriggerInterval -= DeltaTime;
		if (TriggerInterval <= 0.f)
		{
			TriggerInterval = 0.f;
		}
	}
}

void USimplePlayerItemInterComponent::Trigger(const FInputActionValue& Value)
{
	if (TriggerInterval <= 0.0f && SelectingItem.IsValid())
	{
		TriggerInterval = 0.5f;

		StartTriggerSelectingItem();
	}
}

void USimplePlayerItemInterComponent::Throw(const FInputActionValue& Value)
{
	if (TriggerInterval <= 0.0f && InteractingItem.IsValid())
	{
		TriggerInterval = 0.5f;

		EndTriggerInteractingItem();
	}
}

void USimplePlayerItemInterComponent::CheckItemAroundPlayer()
{
	if (!IsStartInteraction())
	{
		SelectingItem = nullptr;
		return;
	}

	UWorld* WorldPtr = GetWorld();
	APlayerController* PlayerControllerPtr = WorldPtr ? WorldPtr->GetFirstPlayerController() : nullptr;
	APawn* PlayerCharacterPtr = PlayerControllerPtr ? PlayerControllerPtr->GetPawn() : nullptr;

	//清空缓存
	NearbyItems.Empty();
	SelectingItem = nullptr;

	if (WorldPtr && PlayerControllerPtr && PlayerCharacterPtr)
	{
		FVector PlayerLocation = PlayerCharacterPtr->GetActorLocation();
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{UEngineTypes::ConvertToObjectType(CheckCollisionType)};
		TArray<AActor*> ActorsToIgnore{PlayerCharacterPtr, InteractingItem.Get()};
		TArray<FHitResult> HitResults;
		if (UKismetSystemLibrary::SphereTraceMultiForObjects(
			WorldPtr,
			PlayerLocation + SphereCenterOffset,
			PlayerLocation + SphereCenterOffset,
			SelectItemActorRange,
			ObjectTypes,
			false,
			ActorsToIgnore,
			DrawDebugTraceMode,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			2.f))
		{
			//找到距离视野正前方最近的物体做交互
			double MinItemAngle = 360.0;
			for (auto& HitResult : HitResults)
			{
				if (ASimpleItemActorBase* HitItemActor = Cast<ASimpleItemActorBase>(HitResult.GetActor()))
				{
					if (HitItemActor->IsStartTrigger(this, false))
					{
						NearbyItems.Emplace(HitItemActor);

						FVector OutLocation{};
						FRotator OutRotation{};
						PlayerControllerPtr->GetPlayerViewPoint(OutLocation, OutRotation);

						//OutRotation.Vector()找到当前的前向向量
						double DotValue = FVector::DotProduct(
							(HitResult.GetActor()->GetActorLocation() - OutLocation).GetSafeNormal(),
							OutRotation.Vector().GetSafeNormal());

						double TmpItemAngle = FMath::RadiansToDegrees(FMath::Acos(DotValue));
						if (TmpItemAngle < MinItemAngle)
						{
							MinItemAngle = TmpItemAngle;
							SelectingItem = HitItemActor;
						}
					}
				}
			}
		}
	}
}

void USimplePlayerItemInterComponent::PickupNewWeapon(ASimpleItemActorWeapon* InNewWeapon, bool bForceInHand)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	if (InNewWeapon == nullptr || InNewWeapon->GetWeaponInstance() == nullptr)
	{
		return;
	}

	if (USimpleWeaponManagerComponent* WeaponManager = GetWeaponManager())
	{
		//在武器槽里找到位置
		//如果有老的武器
		if (ASimpleItemActorWeapon* WeaponInSlot = WeaponManager->GetWeaponInSlot(
			(int32)InNewWeapon->GetWeaponInstance()->GetWeaponSlot()))
		{
			//就把老的武器收起来
			WeaponInSlot->EndTrigger(this, false);
			if (WeaponInSlot == InteractingItem)
			{
				InteractingItem = nullptr;
			}
		}

		//做替换或者装备
		if (InteractingItem.IsValid())
		{
			if (bForceInHand)
			{
				//去掉老的交互新的
				if (InteractingItem->EndTrigger(this, true) &&
					InNewWeapon->StartTrigger(this, true))
				{
					InteractingItem = InNewWeapon;
				}
			}
			else
			{
				InteractingItem->StartTrigger(this, true);
			}
		}
		else
		{
			if (InNewWeapon->StartTrigger(this, true))
			{
				InteractingItem = InNewWeapon;
			}
		}
	}
}

USimpleWeaponManagerComponent* USimplePlayerItemInterComponent::GetWeaponManager() const
{
	if (GetOwner())
	{
		return GetOwner()->FindComponentByClass<USimpleWeaponManagerComponent>();
	}

	return nullptr;
}

void USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem,
	bool bForceInHand)
{
	if (InSelectingItem == nullptr)
	{
		return;
	}

	//手里有东西
	if (InteractingItem == nullptr)
	{
		//如果是物品类
		if (InSelectingItem->GetItemType() == ESimpleKitItemType::ITEM_PICKABLE)
		{
			if (ASimpleItemActorPickable* PickableItem = Cast<ASimpleItemActorPickable>(InSelectingItem))
			{
				switch (PickableItem->GetPickableType())
				{
					//如果是库存物
				case ESimpleKitItemPickableType::ITEM_INVENTORY:
					{
						if (InSelectingItem->StartTrigger(this, bForceInHand))
						{
							//拿到手上
							if (bForceInHand)
							{
								//强制和手上的东西，比如武器做了交换
								InteractingItem = InSelectingItem;
							}
						}
						break;
					}
					//如果是武器
				case ESimpleKitItemPickableType::ITEM_WEAPON:
					{
						if (ASimpleItemActorWeapon* WeaponItem = Cast<ASimpleItemActorWeapon>(PickableItem))
						{
							PickupNewWeapon(WeaponItem, bForceInHand);
						}
						break;
					}
				default:
					{
						if (InSelectingItem->StartTrigger(this, bForceInHand))
						{
							InteractingItem = InSelectingItem;
						}
						break;
					}
				}
			}
		}
		//如果是场景交互物品
		else if (InSelectingItem->GetItemType() == ESimpleKitItemType::ITEM_SCENEINTERACTIVE)
		{
			//防止一次交互两个场景交互物品，比如同时开门和开灯
			if (InteractingItem->GetItemType() == ESimpleKitItemType::ITEM_SCENEINTERACTIVE)
			{
				return;
			}
			else if (InteractingItem->EndTrigger(this, true) && InSelectingItem->StartTrigger(this))
			{
				InteractingItem = InSelectingItem;
			}
			else
			{
				UE_LOG(LogTemp, Error,
					   TEXT(
						   "[USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation]触发\'SceneInteractive\'TPItem错误啦>w<"
					   ))
			}
		}
	}
	//手里没东西
	else
	{
		switch (InSelectingItem->GetItemType())
		{
			//场景交互物品
		case ESimpleKitItemType::ITEM_SCENEINTERACTIVE:
			{
				//防止一次交互两个场景交互物品，比如同时开门和开灯
				if (InteractingItem->GetItemType() == ESimpleKitItemType::ITEM_SCENEINTERACTIVE)
				{
					return;
				}
				else if (InteractingItem->EndTrigger(this, true) && InSelectingItem->StartTrigger(this))
				{
					InteractingItem = InSelectingItem;
				}
				else
				{
					UE_LOG(LogTemp, Error,
					       TEXT(
						       "[USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation]触发\'SceneInteractive\'TPItem错误啦>w<"
					       ))
				}
			}
			//拾取物
		case ESimpleKitItemType::ITEM_PICKABLE:
			{
				if (ASimpleItemActorPickable* PickableItem = Cast<ASimpleItemActorPickable>(InSelectingItem))
				{
					switch (PickableItem->GetPickableType())
					{
						//武器
					case ESimpleKitItemPickableType::ITEM_WEAPON:
						{
							if (ASimpleItemActorWeapon* WeaponItem = Cast<ASimpleItemActorWeapon>(PickableItem))
							{
								PickupNewWeapon(WeaponItem, bForceInHand);
							}
							break;
						}
						//库存
					case ESimpleKitItemPickableType::ITEM_INVENTORY:
						{
							if (ASimpleItemActorInventory* InventoryItem = Cast
								<ASimpleItemActorInventory>(PickableItem))
							{
								switch (InventoryItem->GetInventoryType())
								{
								case ESimpleItemInventoryType::ITEM_INVALID:
									{
										UE_LOG(LogTemp, Error,
										       TEXT(
											       "[USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation]触发\'PickableInvalid\'TPItem错误啦>w<"
										       ))
										break;
									}
								default:
									//是否强制显示在手上
									if (bForceInHand == false)
									{
										//直接触发交互，之后收藏到背包
										InventoryItem->StartTrigger(this, false);
									}
									//上一个释放掉，这一个做新的触发
									//对物品做添加并删除原来的实例
									else if (InteractingItem->EndTrigger(this, true) &&
										InventoryItem->StartTrigger(this, true))
									{
										InteractingItem = InSelectingItem;
									}
									break;
								}
							}
						}
					case ESimpleKitItemPickableType::ITEM_NORMAL:
						{
							if (InteractingItem->EndTrigger(this, true) && InSelectingItem->StartTrigger(this))
							{
								InteractingItem = InSelectingItem;
							}
							else
							{
								UE_LOG(LogTemp, Error,
								       TEXT(
									       "[USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation]触发\'PickupNormal\'TPItem错误啦>w<"
								       ))
							}
							break;
						}
					default:
						{
							break;
						}
					}
				}
			}
		default:
			break;
		}
	}
}

void USimplePlayerItemInterComponent::OnInteractingItemTriggerEnd_Implementation(
	ASimpleItemActorBase* InInteractingItem, bool bIsPutPack)
{
	//交互物品清空
	//弱指针需要IsValid
	if (InteractingItem.IsValid() && InteractingItem->EndTrigger(this, bIsPutPack))
	{
		InteractingItem = nullptr;
	}
}
