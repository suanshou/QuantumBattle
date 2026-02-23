// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ItemDefinition/SimpleItemDefinition.h"

USimpleItemDefinition::USimpleItemDefinition()
{
	//修复VR位置初始化失败的Bug
	ItemRelativeTransform = FTransform::Identity;
}
