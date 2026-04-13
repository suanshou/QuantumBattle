// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraLogChannels.h"
#include "GameFramework/Actor.h"

//实现日志类
DEFINE_LOG_CATEGORY(LogLyra);
DEFINE_LOG_CATEGORY(LogLyraExperience);
DEFINE_LOG_CATEGORY(LogLyraAbilitySystem);
DEFINE_LOG_CATEGORY(LogLyraTeams);

FString GetClientServerContextString(UObject* ContextObject)
{
	//预设默认角色
	ENetRole Role = ROLE_None;

	//尝试转化为Actor，转化成功就获取本地角色
	if (AActor* Actor = Cast<AActor>(ContextObject))
	{
		Role = Actor->GetLocalRole();
	}
	//尝试转化为Component，转化成功就获取本地角色
	else if (UActorComponent* Component = Cast<UActorComponent>(ContextObject))
	{
		Role = Component->GetOwnerRole();
	}

	//返回对应的网络角色
	if (Role != ROLE_None)
	{
		return (Role == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
	}
	else
	{
#if WITH_EDITOR
		if (GIsEditor)
		{
			//编辑器模式（PIE）切换不同游戏场景时会启用此调试工具集
			//在UpdatePlayInEditorWorldDebugString中设置值
			extern ENGINE_API FString GPlayInEditorContextString;
			return GPlayInEditorContextString;
		}
#endif
	}

	//返回默认框框
	return TEXT("[]");
}
