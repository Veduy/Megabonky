// Copyright Epic Games, Inc. All Rights Reserved.

#include "MgbDebugTool.h"

#include "SMgbDebuggerWidget.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "FMgbDebugToolModule"

void FMgbDebugToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	// 메뉴에 버튼 추가
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, nullptr,
		FMenuExtensionDelegate::CreateRaw(this, &FMgbDebugToolModule::AddMenuEntry));
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);


	// 탭 등록
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		"MgbDebugger",
		FOnSpawnTab::CreateRaw(this, &FMgbDebugToolModule::OnSpawnMgbTab))
		.SetDisplayName(NSLOCTEXT("MgbDebugger", "TabTitle", "Mgb Debugger"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMgbDebugToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("MgbDebugger");
}

TSharedRef<class SDockTab> FMgbDebugToolModule::OnSpawnMgbTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SMgbDebuggerWidget)
		];
}

void FMgbDebugToolModule::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		LOCTEXT("OpenTab", "Open Mgb Debugger"),
		LOCTEXT("OpenTabTooltip", "Opens the Mgb Debugger window"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FMgbDebugToolModule::OpenTab)));
}

void FMgbDebugToolModule::OpenTab()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FTabId("MgbDebugger"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMgbDebugToolModule, MgbDebugTool)