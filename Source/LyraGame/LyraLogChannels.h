// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

//声明日志类
//放在同一个地方方便查找
LYRAGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLyra, Log, All);
LYRAGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLyraExperience, Log, All);
LYRAGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLyraAbilitySystem, Log, All);
LYRAGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLyraTeams, Log, All);

//传递一个上下文对象，判定当前的世界是客户端还是服务端
//在LyraExperienceManagerComponent调用，用来打印Experience加载日志，传递的对象是GameState，该对象有网络同步属性
LYRAGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
