// Copyright Epic Games, Inc. All Rights Reserved.

#include "MgbDebugTool.h"

#include "SMgbDebuggerWidget.h"
#include "LevelEditor.h"
#include "HAL/IConsoleManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "EpicUnrealMCPBridge.h"
#include "Async/Async.h"

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


	// 콘솔 명령 등록 (에디터 상시 사용 가능) 
	// 사용법: Mgb.Cmd <CommandType> [Key=Value ...]
	// 예: Mgb.Cmd get_actors_in_level
	// 예: Mgb.Cmd find_actors_by_name pattern=Cube
	// 예: Mgb.Cmd spawn_actor type=StaticMeshActor name=MyActor
	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Mgb.Cmd"),
		TEXT("Execute MCP command locally. Usage: Mgb.Cmd <CommandType> [Key=Value ...]"),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
		{
			if (Args.Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Usage: Mgb.Cmd <CommandType> [Key=Value ...]"));
				return;
			}

			const FString CommandType = Args[0];

			TSharedPtr<FJsonObject> Params = MakeShared<FJsonObject>();
			for (int32 i = 1; i < Args.Num(); ++i)
			{
				FString Key, Value;
				if (Args[i].Split(TEXT("="), &Key, &Value))
				{
					Params->SetStringField(Key, Value);
				}
			}

			UEpicUnrealMCPBridge* Bridge = GEditor->GetEditorSubsystem<UEpicUnrealMCPBridge>();
			if (!Bridge)
			{
				UE_LOG(LogTemp, Error, TEXT("Mgb.Cmd: UEpicUnrealMCPBridge not found"));
				return;
			}

			// ExecuteCommand 내부에서 AsyncTask(GameThread)를 사용하므로
			// 콘솔 명령(GameThread)에서 직접 호출하면 데드락 발생.
			// 백그라운드 스레드에서 호출하여 우회.
			Async(EAsyncExecution::Thread, [Bridge, CommandType, Params]()
			{
				FString Result = Bridge->ExecuteCommand(CommandType, Params);

				AsyncTask(ENamedThreads::GameThread, [CommandType, Result]()
				{
					UE_LOG(LogTemp, Log, TEXT("Mgb.Cmd [%s] Result:\n%s"), *CommandType, *Result);
				});
			});
		}),
		ECVF_Default
	));

	// 탭 등록
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		"MgbDebugger",
		FOnSpawnTab::CreateRaw(this, &FMgbDebugToolModule::OnSpawnMgbTab))
		.SetDisplayName(NSLOCTEXT("MgbDebugger", "TabTitle", "Mgb Debugger"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMgbDebugToolModule::ShutdownModule()
{
	for (IConsoleObject* Cmd : ConsoleCommands)
	{
		IConsoleManager::Get().UnregisterConsoleObject(Cmd);
	}
	ConsoleCommands.Empty();

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