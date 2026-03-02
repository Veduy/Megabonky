// Copyright is owned by Veduy.


#include "MgbSubsystem.h"
#include "JsonUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemGlobals.h"
#include "Core/MgbTitleController.h"


// Static constant initialization
const FString UMgbSubsystem::SaveSlotName = TEXT("MegabonkyPlayerData");
const int32 UMgbSubsystem::SaveUserIndex = 0;

void UMgbSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	HttpModule = &FHttpModule::Get();

	// 이 코드가 없으면 빌드 버전에서 GameplayCue Manager가 작동하지 않습니다.
	UAbilitySystemGlobals::Get().InitGlobalData();

	// Load saved data on game start
	LoadGameData();
}

void UMgbSubsystem::Deinitialize()
{

}

void UMgbSubsystem::RequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		return;
	}

	int32 StatusCode = Response->GetResponseCode();
	FString ResponseContent = Response->GetContentAsString();

	UE_LOG(LogTemp, Warning, TEXT("Code : %d, %s"), StatusCode, *(ResponseContent));

	auto JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);

	TSharedPtr<FJsonObject> JsonObject;
	FJsonSerializer::Deserialize(JsonReader, JsonObject);

	auto Result = JsonObject->GetField(TEXT("result"), EJson::Boolean);
	bool bResult = Result.IsValid() && Result->AsBool();

	if (bResult)
	{
		auto ReturnedID = JsonObject->GetField(TEXT("name"), EJson::String); // Note: server currently repurposes 'name' property in CreateJsonResponse
		UE_LOG(LogTemp, Warning, TEXT("Login Success - user_id: %s"), *ReturnedID->AsString());
		
		CurrentUserID = ReturnedID->AsString();

		auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			auto TitlePC = Cast<AMgbTitleController>(PC);
			if (TitlePC)
			{
				TitlePC->ServerStartGame();
				UE_LOG(LogTemp, Warning, TEXT("Starting Game..."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Failed"));
	}
}

void UMgbSubsystem::Login(bool bTest)
{
	if (bTest)
	{
		auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			auto TitlePC = Cast<AMgbTitleController>(PC);
			if (TitlePC)
			{
				TitlePC->ServerStartGame();
				UE_LOG(LogTemp, Warning, TEXT("Starting Game..."));
			}
		}
	}
	
	auto Request = HttpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMgbSubsystem::RequestCompleted);

	FString ID = "admin";
	FString Passwd = "1234";

	// JSON body로 ID, Passwd 전송
	FString JsonBody = FString::Printf(TEXT("{\"user_id\": \"%s\", \"passwd\": \"%s\"}"), *ID, *Passwd);

	Request->SetURL(TEXT("http://127.0.0.1:8080/api/login"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(JsonBody);

	Request->ProcessRequest();
}

void UMgbSubsystem::SaveGameSession(const FGameSessionRecord& SessionRecord)
{
	if (!CurrentSaveData)
	{
		CurrentSaveData = NewObject<UMgbSaveGame>();
	}

	// Update best records
	UpdateBestRecords(CurrentSaveData, SessionRecord);

	// Accumulate total weapon kills
	UpdateTotalWeaponKills(CurrentSaveData, SessionRecord.WeaponKills);

	// Increment total games played
	CurrentSaveData->TotalGamesPlayed++;

	// Save to file
	bool bSaveSuccess = UGameplayStatics::SaveGameToSlot(CurrentSaveData, SaveSlotName, SaveUserIndex);
	if (bSaveSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game session saved successfully: %lld kills, %d weapons"),
			SessionRecord.TotalKills, SessionRecord.WeaponKills.Num());
			
		// 혹시 에디터에서 바로 시작해서 로그인을 안 거친 경우를 위한 fallback
		if (CurrentUserID.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("CurrentUserID is empty! Fallback to 'admin' for testing."));
			CurrentUserID = TEXT("admin");
		}
		
		auto Request = HttpModule->CreateRequest();
		FString JsonBody = FString::Printf(TEXT("{\"user_id\": \"%s\", \"total_kills\": %lld}"), *CurrentUserID, SessionRecord.TotalKills);

		Request->SetURL(TEXT("http://127.0.0.1:8080/api/save_kills"));
		Request->SetVerb(TEXT("POST"));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetContentAsString(JsonBody);
		
		Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bProcessedSuccessfully) {
			if (bProcessedSuccessfully && Res.IsValid())
			{
				UE_LOG(LogTemp, Log, TEXT("Save kills to DB response: %s"), *Res->GetContentAsString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Save kills to DB Request Failed!"));
			}
		});

		bool bSent = Request->ProcessRequest();
		UE_LOG(LogTemp, Log, TEXT("ProcessRequest for save_kills called. Return: %d"), bSent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save game session!"));
	}
}

UMgbSaveGame* UMgbSubsystem::LoadGameData()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
	{
		CurrentSaveData = Cast<UMgbSaveGame>(
			UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));

		if (CurrentSaveData)
		{
			UE_LOG(LogTemp, Warning, TEXT("Loaded game data: %d total games, %d weapon types"),
				CurrentSaveData->TotalGamesPlayed, CurrentSaveData->TotalWeaponKills.Num());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Save file corrupted, creating new one"));
			CurrentSaveData = NewObject<UMgbSaveGame>();
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No save file found, creating new one"));
		CurrentSaveData = NewObject<UMgbSaveGame>();
	}

	return CurrentSaveData;
}

void UMgbSubsystem::UpdateBestRecords(UMgbSaveGame* InOutSaveData, const FGameSessionRecord& NewRecord)
{
	if (!InOutSaveData)
	{
		return;
	}

	// Update best kill record
	if (NewRecord.TotalKills > InOutSaveData->BestRecord_Kills.TotalKills)
	{
		InOutSaveData->BestRecord_Kills = NewRecord;
		UE_LOG(LogTemp, Log, TEXT("New best kill record: %lld kills"), NewRecord.TotalKills);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Best kill record remains: %lld kills"), InOutSaveData->BestRecord_Kills.TotalKills);
	}
}

void UMgbSubsystem::UpdateTotalWeaponKills(UMgbSaveGame* InOutSaveData, const TArray<FWeaponKillRecord>& WeaponKills)
{
	if (!InOutSaveData)
	{
		return;
	}

	// Accumulate weapon kills to total statistics
	for (const FWeaponKillRecord& WeaponKill : WeaponKills)
	{
		if (int64* TotalKills = InOutSaveData->TotalWeaponKills.Find(WeaponKill.WeaponName))
		{
			*TotalKills += WeaponKill.KillCount;
			UE_LOG(LogTemp, Log, TEXT("Updated total kills for %s: %lld"), *WeaponKill.WeaponName.ToString(), *TotalKills);
		}
		else
		{
			InOutSaveData->TotalWeaponKills.Add(WeaponKill.WeaponName, WeaponKill.KillCount);
			UE_LOG(LogTemp, Log, TEXT("Added new weapon %s with kills: %lld"), *WeaponKill.WeaponName.ToString(), WeaponKill.KillCount);
		}
	}
}