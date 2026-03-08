// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Actor/Item/Pickable/Weapons/SimpleItemActorWeapon.h"
#include "Components/ActorComponent.h"
#include "SimpleWeaponManagerComponent.generated.h"

class ASimpleItemActorWeapon;

UCLASS(ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimpleWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimpleWeaponManagerComponent();

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	void InitWeaponManager(int32 InSlotSize);

	UFUNCTION(BlueprintCallable, Category="Weapon Manager Component")
	TArray<ASimpleItemActorWeapon*> GetWeapons() const { return Weapons; }

	UFUNCTION(BlueprintCallable, Category="Weapon Manager Component")
	ASimpleItemActorWeapon* GetEquippingWeapon() const { return EquippingWeapon.Get(); }

	//武器第二步
	//在插槽找到对应武器
	UFUNCTION(BlueprintCallable, Category="Weapon Manager Component")
	ASimpleItemActorWeapon* GetWeaponInSlot(int32 WeaponSlot);

	UFUNCTION(BlueprintCallable, Category="Weapon Manager Component")
	int32 GetEquipSlot() const { return EquipSlot; }

	//在没有找到插槽武器位置时，OnStartTrigger后
	//武器的增删，改和替换
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	bool AddWeapon(ASimpleItemActorWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	bool RemoveWeapon(int32 InRemoveSlot);

	//装备武器
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	bool EquipWeapon(int32 InSlot);

	//卸载武器
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	bool UnequipWeapon();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	bool ReplaceWeapon(ASimpleItemActorWeapon* NewWeapon);

protected:
	UFUNCTION()
	void OnRep_EquipSlot(const int32& OldEquipSlot);

	UFUNCTION()
	void OnRep_EquippingWeapon(const TWeakObjectPtr<ASimpleItemActorWeapon>& OldWeapon);

protected:
	//给蓝图的接口，告诉蓝图可以更新装备
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="EquipSlotRepNotify"))
	void K2_EquipSlotRepNotify(const int32& OldEquipSlot);

protected:
	//通过武器获取到对应的Slot
	bool GetWeaponSlot(const ASimpleItemActorWeapon* InWeapon, int32& OutSlot);

	//武器装载和卸载时的触发逻辑
	void OnWeaponEquipped(ASimpleItemActorWeapon* EquippedWeapon);
	void OnWeaponUnequipped(ASimpleItemActorWeapon* UnequippedWeapon);

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	//装备的插槽
	UPROPERTY(ReplicatedUsing="OnRep_EquipSlot", BlueprintReadOnly, Category="Weapon Manager Component")
	int32 EquipSlot = -1;

	//装备的武器
	UPROPERTY(ReplicatedUsing="OnRep_EquippingWeapon", BlueprintReadOnly, Category="Weapon Manager Component")
	TWeakObjectPtr<ASimpleItemActorWeapon> EquippingWeapon;

	//拥有的武器
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Weapon Manager Component")
	TArray<ASimpleItemActorWeapon*> Weapons;

private:
	//武器的技能
	//typetraits不支持前置声明
	TArray<FGameplayAbilitySpecHandle> WeaponGameAbilities;
};
