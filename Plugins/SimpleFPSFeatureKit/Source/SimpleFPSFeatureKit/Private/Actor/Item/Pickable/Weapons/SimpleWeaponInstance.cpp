// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Pickable/Weapons/SimpleWeaponInstance.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UWorld* USimpleWeaponInstance::GetWorld() const
{
	return UObject::GetWorld();
}

void USimpleWeaponInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, CurrentCartridge);
}

void USimpleWeaponInstance::WeaponTick(float DeltaSeconds)
{
	const bool bMinSpread = UpdateSpread(DeltaSeconds);
	const bool bMinMultiplier = UpdateMultipliers(DeltaSeconds);

	//首发精准的严格判定，设计理念是支持点射而非连射
	bHasFirstShotAccuracy = bAllowFirstShotAccuracy && bMinSpread && bMinMultiplier;
}

void USimpleWeaponInstance::OnEquipped(UObject* InInstigator)
{
	float MinHeatRange{};
	float MaxHeatRange{};
	//避免武器过热或者过冷
	CurrentHeat = (MinHeatRange + MaxHeatRange) * 0.5f;

	//保证武器过热在数学和物理上的一致性
	CurrentSpreadAngle = HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);

	CurrentSpreadAngleMultiplier = 1.f;
	StandingStillMultiplier = 1.f;
	CrouchingMultiplier = 1.f;

	SetAnimationParamOnEquipped(InInstigator);
}

void USimpleWeaponInstance::OnUnequipped(UObject* InInstigator)
{
	SetAnimationParamOnUnequipped(InInstigator);
}

void USimpleWeaponInstance::AddSpread()
{
}

void USimpleWeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World);
	LastFireTime = World->GetTimeSeconds();
}

bool USimpleWeaponInstance::UpdateSpread(float DeltaSeconds)
{
	return false;
}

bool USimpleWeaponInstance::UpdateMultipliers(float DeltaSeconds)
{
	return false;
}

void USimpleWeaponInstance::CartridgeCast(int32 CastCounts)
{
	check(CastCounts <= CurrentCartridge);

	CurrentCartridge -= CastCounts;
}

void USimpleWeaponInstance::ReloadCartridge(int32 CartridgeCounts)
{
	CurrentCartridge += CartridgeCounts;

	check(CurrentCartridge <= ClipSize);
}

ACharacter* USimpleWeaponInstance::GetCharacter() const
{
	return Cast<ACharacter>(Instigator);
}

void USimpleWeaponInstance::ComputeSpreadRange(float& MinSpread, float& MaxSpread)
{
	HeatToSpreadAngleCurve.GetRichCurveConst()->GetValueRange(MinSpread, MaxSpread);
}

void USimpleWeaponInstance::ComputeHeatRange(float& MinHeat, float& MaxHeat)
{
}

USimpleWeaponInstance::USimpleWeaponInstance()
{
}
