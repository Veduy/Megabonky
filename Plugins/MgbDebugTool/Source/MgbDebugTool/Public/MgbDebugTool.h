// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FMenuBuilder;

class FMgbDebugToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	TSharedRef<class SDockTab> OnSpawnMgbTab(const class FSpawnTabArgs& SpawnTabArgs);

	void AddMenuEntry(FMenuBuilder& MenuBuilder);

	void OpenTab();
};
