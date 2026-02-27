// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SimpleFPSFeatureKitType.h"
#include "UObject/Object.h"
#include "SimpleWeaponInstance.generated.h"

/**
 * 
 */

class UGameplayAbility;

USTRUCT(BlueprintType)
struct SIMPLEFPSFEATUREKIT_API FSimpleWeaponBindingAbility
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Ability")
	TSubclassOf<UGameplayAbility> WeaponAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Ability")
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct SIMPLEFPSFEATUREKIT_API FSimpleWeaponEquippedMontage
{
	GENERATED_USTRUCT_BODY()

	FSimpleWeaponEquippedMontage()
	{
		PlayRate = 1.f;
		bStopAllMontage = true;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	TObjectPtr<UAnimMontage> AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	FName TransactionName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	float PlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	bool bStopAllMontage;
};

enum class ESimpleWeaponSlot : uint8;

UCLASS()
class SIMPLEFPSFEATUREKIT_API USimpleWeaponInstance : public UObject
{
	GENERATED_BODY()

protected:
	//武器插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	ESimpleWeaponSlot WeaponSlot = ESimpleWeaponSlot::WS_INVALID;

	//装备的槽位名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	FName InventorySlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	FTransform InventoryRelativeTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	TArray<FSimpleWeaponBindingAbility> WeaponBindingAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	FSimpleWeaponEquippedMontage EquippedMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	FSimpleWeaponEquippedMontage UnequippedMontage;

	//初始化的弹夹数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	int32 CartridgeCountsOnSpawn = 0;

	//单个弹夹子弹数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	int32 ClipSize = 10;

	//每发的子弹数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	int32 BulletsPerCartridge = 1;

	//最大伤害距离
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	float MaxDamageRange = 25000.f;

	//子弹碰撞半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	float BulletRadius = 4.f;

	//伤害的距离衰减曲线
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	FRuntimeFloatCurve DistanceDamageFalloffCurve;

	//材质对应的伤害倍率，用于爆头加伤
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	TMap<FName, float> MaterialDamageMultiplier;

	//武器散射与热量系统（Spread&Heat）
	//散射角指数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float SpreadExponent = 1.f;

	//热量到散射角的映射，热量高时有更高的后座惩罚
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	FRuntimeFloatCurve HeatToSpreadAngleCurve;

	//模拟武器升温
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	FRuntimeFloatCurve HeatToHeatPerShotCurve;

	//模拟武器冷却
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	FRuntimeFloatCurve HeatToCoolDownPerSecondCurve;

	//停止射击后恢复到原精度的延迟时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float SpreadRecoveryCoolDownDelay = 0.25f;

	//允许首发精准
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	bool bAllowFirstShotAccuracy = true;

	//散射角度倍率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float SpreadAngleMultiplier_Aiming = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float SpreadAngleMultiplier_StandingStill = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float SpreadAngleMultiplier_Crouching = 1.f;

	//从移动到静止的散射平滑
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float TransitionRate_StandingStill = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float TransitionRate_Crouching = 5.f;

	//速度小于这个阈值时判定角色站着不动，更加精准
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float StandingStillSpeedThreshold = 20.f;

	//站立到移动的速度过渡范围
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float StandingStillToMovingSpeedRange = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	bool bDrawDebug = false;

private:
	//继承自哪里
	UPROPERTY(Replicated)
	UObject* Instigator;

	//当前子弹数
	UPROPERTY(Replicated)
	int32 CurrentCartridge = 0;

private:
	//上次连射的时间
	double LastFireTime = 0.0;
	//累计的热量
	float CurrentHeat = 0.f;
	//当前的散射角度
	float CurrentSpreadAngle = 0.f;
	//射击是否触发了首发精准
	bool bHasFirstShotAccuracy = false;
	//当前散射角的最终倍率
	float CurrentSpreadAngleMultiplier = 1.f;
	//角色静止时的散射倍率
	float StandingStillMultiplier = 1.f;
	//角色蹲下时的散射倍率
	float CrouchingMultiplier = 1.f;

public:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//每帧更新武器状态
	void WeaponTick(float DeltaSeconds);

	void OnEquipped(UObject* InInstigator);
	void OnUnequipped(UObject* InInstigator);

	void AddSpread();
	void UpdateFiringTime();

public:
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	ESimpleWeaponSlot GetWeaponSlot() const { return WeaponSlot; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FName GetInventorySlotName() const { return InventorySlotName; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FTransform GetInventoryRelativeTransform() const { return InventoryRelativeTransform; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	TArray<FSimpleWeaponBindingAbility> GetWeaponBindingAbilities() const { return WeaponBindingAbilities; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FSimpleWeaponEquippedMontage GetEquippedMontage() const { return EquippedMontage; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FSimpleWeaponEquippedMontage GetUnequippedMontage() const { return UnequippedMontage; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	int32 GetCurrentCartridge() const { return CurrentCartridge; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	int32 GetClipSize() const { return ClipSize; }

	int32 GetBulletPerCartridge() const { return BulletsPerCartridge; }
	float GetCalculatedSpreadAngle() const { return CurrentSpreadAngle; }

	float GetCalculatedSpreadAngleMultiplier() const
	{
		return bHasFirstShotAccuracy ? 0.f : CurrentSpreadAngleMultiplier;
	}

	bool GetHasFirstShotAccuracy() const { return bHasFirstShotAccuracy; }
	float GetSpreadExponent() const { return SpreadExponent; }
	float GetMaxDamageDistance() const { return MaxDamageRange; }
	float GetBulletRadius() const { return BulletRadius; }
	bool GetCanDrawDebug() const { return bDrawDebug; }

	//所有动画相关功能集中到蓝图去实现，符合单一职责原则
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon Animation")
	void SetAnimationParamOnEquipped(UObject* InInstigator);
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon Animation")
	void SetAnimationParamOnUnequipped(UObject* InInstigator);

	inline float ClampFloat(float NewHeat)
	{
		float MinHeat;
		float MaxHeat;
		ComputeHeatRange(MinHeat, MaxHeat);

		return FMath::Clamp(NewHeat, MinHeat, MaxHeat);
	}

	bool UpdateSpread(float DeltaSeconds);
	bool UpdateMultipliers(float DeltaSeconds);

public:
	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }
	//减少子弹
	void CartridgeCast(int32 CastCounts);
	//换弹
	void ReloadCartridge(int32 CartridgeCounts);

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	UObject* GetInstigator() const { return Instigator; }

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	ACharacter* GetCharacter() const;

private:
	void ComputeSpreadRange(float& MinSpread, float& MaxSpread);
	void ComputeHeatRange(float& MinHeat, float& MaxHeat);

public:
	//在实例化时的例如添加弹药等等逻辑
	void InitInstance()
	{
		CurrentCartridge = CartridgeCountsOnSpawn;
	}

public:
	USimpleWeaponInstance();
};
