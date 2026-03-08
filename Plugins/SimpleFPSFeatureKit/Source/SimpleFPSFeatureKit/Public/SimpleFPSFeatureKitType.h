#pragma once

#include "CoreMinimal.h"
#include "SimpleFPSFeatureKitType.generated.h"

UENUM(BlueprintType)
enum class ESimpleKitItemType : uint8
{
	//拾取物
	ITEM_PICKABLE UMETA(DisplayName = "Pickable"),
	//场景交互物品
	ITEM_SCENEINTERACTIVE UMETA(DisplayName = "Scene Interactive"),
	ITEM_INVALID UMETA(DisplayName = "Invalid", Hidden),
};

UENUM(BlueprintType)
enum class ESimpleKitItemPickableType : uint8
{
	//普通物品
	ITEM_NORMAL UMETA(DisplayName = "Normal"),
	//库存物
	ITEM_INVENTORY UMETA(DisplayName = "Inventory"),
	//武器
	ITEM_WEAPON UMETA(DisplayName = "Weapon"),
	ITEM_INVALID UMETA(DisplayName = "Invalid", Hidden),
};

UENUM(BlueprintType)
enum class ESimpleItemInventoryType : uint8
{
	//普通物品
	ITEM_NORMAL UMETA(DisplayName = "Normal"),
	//补给物品
	ITEM_SUPPLY UMETA(DisplayName = "Supply"),
	ITEM_INVALID UMETA(DisplayName = "Invalid", Hidden),
};

UENUM(BlueprintType)
enum class ESimpleWeaponSlot : uint8
{
	//手枪
	WS_PISTOL = 0 UMETA(DisplayName = "Pistol"),
	//步枪
	WS_RIFLE = 1 UMETA(DisplayName = "Rifle"),
	//冲锋枪
	WS_SMG = 2 UMETA(DisplayName = "Smg"),
	//狙击枪
	WS_SNIPPER = 3 UMETA(DisplayName = "Snipper"),
	WS_INVALID = 255 UMETA(DisplayName = "Invalid", Hidden),
};
