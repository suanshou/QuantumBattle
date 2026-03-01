// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Pickable/Weapons/SimpleWeaponInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	float MinHeatRange{}, MaxHeatRange{};
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
	const float HeatPerShot = HeatToHeatPerShotCurve.GetRichCurveConst()->Eval(CurrentHeat);
	CurrentHeat = ClampHeat(CurrentHeat + HeatPerShot);

	CurrentSpreadAngle = HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);
}

void USimpleWeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World);
	LastFireTime = World->GetTimeSeconds();
}

bool USimpleWeaponInstance::UpdateSpread(float DeltaSeconds)
{
	const float TimeSinceFired = GetWorld() ? GetWorld()->TimeSince(LastFireTime) : 0.f;

	//超过冷却延迟则开始冷却
	if (TimeSinceFired > SpreadRecoveryCoolDownDelay)
	{
		const float CoolDownRate = HeatToCoolDownPerSecondCurve.GetRichCurveConst()->Eval(CurrentHeat);
		CurrentHeat = ClampHeat(CurrentHeat - (CoolDownRate * DeltaSeconds));

		CurrentSpreadAngle = HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);
	}

	float MinSpread, MaxSpread;
	ComputeSpreadRange(MinSpread, MaxSpread);

	return FMath::IsNearlyEqual(CurrentSpreadAngle, MinSpread,KINDA_SMALL_NUMBER);
}

bool USimpleWeaponInstance::UpdateMultipliers(float DeltaSeconds)
{
	const float MultiplierNearlyEqualThreshold = 0.05f;

	ACharacter* PlayerCharacter = GetCharacter();
	UCharacterMovementComponent* CharMoveComp = PlayerCharacter
		                                            ? PlayerCharacter->GetCharacterMovement()
		                                            : nullptr;
	const float CharacterSpeed = PlayerCharacter ? PlayerCharacter->GetVelocity().Size() : 0.f;

	//在不同移动状态呈现不同的精准度，角色速度越高，射击精准度越低
	//GetMappedRangeValueClamped，对范围内的给定值输出百分比
	const float MovementTargetValue = FMath::GetMappedRangeValueClamped(
		FVector2D(StandingStillSpeedThreshold, StandingStillSpeedThreshold + StandingStillToMovingSpeedRange),
		FVector2D(SpreadAngleMultiplier_StandingStill, 1.f),
		CharacterSpeed);

	//站立状态的数据
	//FInterpTo，用于平滑，贝塞尔曲线，在终点附近无限接近于目标
	StandingStillMultiplier = FMath::FInterpTo(StandingStillMultiplier, MovementTargetValue, DeltaSeconds,
	                                           TransitionRate_StandingStill);
	const bool bStandingStillMultiplierAtMin = FMath::IsNearlyEqual(StandingStillMultiplier,
	                                                                SpreadAngleMultiplier_StandingStill,
	                                                                MultiplierNearlyEqualThreshold);

	//蹲下状态的数据
	const bool bIsCrouching = (CharMoveComp != nullptr) && CharMoveComp->IsCrouching();
	const float CrouchingTargetValue = bIsCrouching ? SpreadAngleMultiplier_Crouching : 1.f;
	CrouchingMultiplier = FMath::FInterpTo(CrouchingMultiplier, CrouchingTargetValue, DeltaSeconds,
	                                       TransitionRate_Crouching);
	const bool bCrouchingMultiplierAtMin = FMath::IsNearlyEqual(CrouchingMultiplier,
	                                                            SpreadAngleMultiplier_Crouching,
	                                                            MultiplierNearlyEqualThreshold);

	//开镜时的数据
	//开镜会降低散射
	float AimingAlpha = 0.f;
	const float AimingMultiplier = FMath::GetMappedRangeValueClamped(
		FVector2D(0.f, 1.f),
		FVector2D(1.f, SpreadAngleMultiplier_Aiming),
		AimingAlpha);
	const bool bAimingMultiplierAtMin = FMath::IsNearlyEqual(AimingMultiplier,
	                                                         SpreadAngleMultiplier_Aiming,
	                                                        MultiplierNearlyEqualThreshold);

	CurrentSpreadAngleMultiplier = StandingStillMultiplier * CrouchingMultiplier * AimingMultiplier;
	
	return bStandingStillMultiplierAtMin && bCrouchingMultiplierAtMin && bAimingMultiplierAtMin;
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
	float Min1, Max1;
	HeatToHeatPerShotCurve.GetRichCurveConst()->GetTimeRange(Min1, Max1);

	float Min2, Max2;
	HeatToCoolDownPerSecondCurve.GetRichCurveConst()->GetTimeRange(Min2, Max2);

	float Min3, Max3;
	HeatToCoolDownPerSecondCurve.GetRichCurveConst()->GetTimeRange(Min3, Max3);

	MinHeat = FMath::Min(Min1, FMath::Min(Min2, Min3));
	MaxHeat = FMath::Max(Max1, FMath::Max(Max2, Max3));
}

USimpleWeaponInstance::USimpleWeaponInstance()
{
}
