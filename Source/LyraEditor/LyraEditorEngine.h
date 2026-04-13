// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Editor/UnrealEdEngine.h"

#include "LyraEditorEngine.generated.h"

class IEngineLoop;
class UObject;


UCLASS()
class ULyraEditorEngine : public UUnrealEdEngine
{
	GENERATED_BODY()

public:
	ULyraEditorEngine(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void Init(IEngineLoop* InEngineLoop) override;
	virtual void Start() override;
	virtual void Tick(float DeltaSeconds, bool bIdleMode) override;

	//在创建PIE实例之前执行
	virtual FGameInstancePIEResult PreCreatePIEInstances(const bool bAnyBlueprintErrors,
	                                                     const bool bStartInSpectatorMode,
	                                                     const float PIEStartTime,
	                                                     const bool bSupportsOnlinePIE,
	                                                     int32& InNumOnlinePIEInstances) override;

private:
	//第一帧执行
	//插件内容强制打开，方便使用GameFeature插件
	void FirstTickSetup();

	//用于控制第一帧事件的变量
	bool bFirstTickSetup = false;
};
