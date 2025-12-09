// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MEGABONKY_API NetworkLog
{
public:
	NetworkLog();
	~NetworkLog();

	static void Log(UWorld* World, const FString& CallInfo, const FString& InString);
};

#define NET_LOG(String)		NetworkLog::Log(GetWorld(), ANSI_TO_TCHAR(__FUNCTION__), String)