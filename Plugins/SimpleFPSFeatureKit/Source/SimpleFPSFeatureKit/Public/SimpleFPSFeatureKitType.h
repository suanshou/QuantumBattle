#pragma once

#include "CoreMinimal.h"
#include "SimpleFPSFeatureKitType.generated.h"

UENUM(BlueprintType)
enum class ESimpleKitItemType : uint8
{
	ITEM_PICKABLE UMETA(DisplayName = "Pickable"),
	ITEM_SCENEINTERACTIVE UMETA(DisplayName = "Scene Interactive"),
	ITEM_INVALID UMETA(DisplayName = "Invalid", Hidden),
};

UENUM(BlueprintType)
enum class ESimpleKitItemPickableType : uint8
{
	ITEM_NORMAL UMETA(DisplayName = "Normal"),
	ITEM_INVENTORY UMETA(DisplayName = "Inventory"),
	ITEM_WEAPON UMETA(DisplayName = "Weapon"),
	ITEM_INVALID UMETA(DisplayName = "Invalid", Hidden),
};

UENUM(BlueprintType)
enum class ESimpleItemInventoryType : uint8
{
	ITEM_NORMAL UMETA(DisplayName = "Normal"),
	ITEM_SUPPLY UMETA(DisplayName = "Supply"),
	ITEM_INVALID UMETA(DisplayName = "Invalid", Hidden),
};
