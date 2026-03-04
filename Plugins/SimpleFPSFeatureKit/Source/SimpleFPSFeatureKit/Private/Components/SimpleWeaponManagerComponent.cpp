// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleWeaponManagerComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Actor/Item/Pickable/Weapons/SimpleWeaponInstance.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


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
	if (Weapons.IsValidIndex(WeaponSlot))
	{
		return Weapons[WeaponSlot];
	}

	return nullptr;
}

bool USimpleWeaponManagerComponent::AddWeapon(ASimpleItemActorWeapon* NewWeapon)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	int32 TargetSlot = INDEX_NONE;

	if (GetWeaponSlot(NewWeapon, TargetSlot))
	{
		if (Weapons[TargetSlot] == nullptr)
		{
			Weapons[TargetSlot] = NewWeapon;

			return true;
		}
	}

	return false;
}

bool USimpleWeaponManagerComponent::RemoveWeapon(int32 InRemoveSlot)
{
	if (Weapons.IsValidIndex(InRemoveSlot) && Weapons[InRemoveSlot])
	{
		if (EquipSlot == InRemoveSlot)
		{
			UnequipWeapon();
		}

		//将装备的武器和对应武器列表元素置空
		Weapons[InRemoveSlot] = nullptr;
		EquippingWeapon = nullptr;

		return true;
	}
	return false;
}

bool USimpleWeaponManagerComponent::EquipWeapon(int32 InSlot)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	if (EquipSlot != InSlot && Weapons.IsValidIndex(InSlot) && Weapons[InSlot])
	{
		OnWeaponEquipped(Weapons[InSlot]);
		EquipSlot = InSlot;
		EquippingWeapon = Weapons[InSlot];
		return true;
	}

	return false;
}

bool USimpleWeaponManagerComponent::UnequipWeapon()
{
	check(GetOwner() && GetOwner()->HasAuthority());

	if (Weapons.IsValidIndex(EquipSlot) && Weapons[EquipSlot])
	{
		OnWeaponUnequipped(Weapons[EquipSlot]);
		//返回到默认值
		EquipSlot = -1;
		EquippingWeapon = nullptr;
		return true;
	}

	return false;
}

bool USimpleWeaponManagerComponent::ReplaceWeapon(ASimpleItemActorWeapon* NewWeapon)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	int32 TargetSlot = INDEX_NONE;
	if (GetWeaponSlot(NewWeapon, TargetSlot))
	{
		if (Weapons[TargetSlot])
		{
			Weapons[TargetSlot] = NewWeapon;
			EquippingWeapon = Weapons[TargetSlot];

			return true;
		}
	}

	return false;
}

void USimpleWeaponManagerComponent::OnRep_EquipSlot(const int32& OldEquipSlot)
{
	//调用蓝图接口
	K2_EquipSlotRepNotify(OldEquipSlot);
}

void USimpleWeaponManagerComponent::OnRep_EquippingWeapon(const TWeakObjectPtr<ASimpleItemActorWeapon>& OldWeapon)
{
	if (OldWeapon.IsValid())
	{
		OnWeaponUnequipped(OldWeapon.Get());
	}

	if (EquippingWeapon.IsValid())
	{
		OnWeaponEquipped(EquippingWeapon.Get());
	}
}

bool USimpleWeaponManagerComponent::GetWeaponSlot(const ASimpleItemActorWeapon* InWeapon, int32& OutSlot)
{
	if (InWeapon && InWeapon->GetWeaponInstance())
	{
		//转换为插槽对应的枚举类
		OutSlot = (int32)InWeapon->GetWeaponInstance()->GetWeaponSlot();
		return Weapons.IsValidIndex(OutSlot);
	}

	return false;
}

void USimpleWeaponManagerComponent::OnWeaponEquipped(ASimpleItemActorWeapon* EquippedWeapon)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponInstance())
	{
		if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner()))
		{
			if (PlayerCharacter->HasAuthority())
			{
				if (IAbilitySystemInterface* InAbilityInterface = Cast<IAbilitySystemInterface>(PlayerCharacter))
				{
					if (UAbilitySystemComponent* ASC = InAbilityInterface->GetAbilitySystemComponent())
					{
						for (const FSimpleWeaponBindingAbility& TmpAbility : EquippingWeapon->GetWeaponInstance()->
						     GetWeaponBindingAbilities())
						{
							FGameplayAbilitySpec Spec = FGameplayAbilitySpec(TmpAbility.WeaponAbility, 1);
							Spec.SourceObject = EquippingWeapon;
							Spec.DynamicAbilityTags.AddTag(TmpAbility.InputTag);

							WeaponGameAbilities.Add(ASC->GiveAbility(Spec));
						}
					}

					//设置施法者
					EquippedWeapon->GetWeaponInstance()->SetInstigator(GetOwner());
					//播放对应动画
					FSimpleWeaponEquippedMontage EquippedMontage = EquippedWeapon->GetWeaponInstance()->
						GetEquippedMontage();
					//在此处是播放动画的逻辑
				}
			}
		}
	}
}

void USimpleWeaponManagerComponent::OnWeaponUnequipped(ASimpleItemActorWeapon* UnequippedWeapon)
{
	if (UnequippedWeapon && UnequippedWeapon->GetWeaponInstance())
	{
		if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner()))
		{
			if (PlayerCharacter->HasAuthority())
			{
				if (IAbilitySystemInterface* InAbilityInterface = Cast<IAbilitySystemInterface>(PlayerCharacter))
				{
					if (UAbilitySystemComponent* ASC = InAbilityInterface->GetAbilitySystemComponent())
					{
						//清空技能
						for (const auto& TmpHandle : WeaponGameAbilities)
						{
							ASC->ClearAbility(TmpHandle);
						}
						//清空数组
						WeaponGameAbilities.Reset();
					}

					//播放对应动画
					FSimpleWeaponEquippedMontage UnequippedMontage = UnequippedWeapon->GetWeaponInstance()->
						GetUnequippedMontage();
					//此处是播放动画的逻辑
				}

				UnequippedWeapon->GetWeaponInstance()->SetInstigator(GetOwner());
			}
		}
	}
}

void USimpleWeaponManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//条件同步，如果服务器更改了值就去调用通知
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, EquipSlot, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, EquippingWeapon, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(ThisClass, Weapons);
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
	if (GetOwner() && GetOwner()->HasLocalNetOwner())
	{
		if (EquippingWeapon.IsValid() && EquippingWeapon->GetWeaponInstance())
		{
			//每帧处理散射和倍率
			EquippingWeapon->GetWeaponInstance()->WeaponTick(DeltaTime);
		}
	}
}
