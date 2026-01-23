// Copyright is owned by Veduy.


#include "MgbSubsystem.h"
#include "JsonUtilities.h"


void UMgbSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	HttpModule = &FHttpModule::Get();
}

void UMgbSubsystem::Deinitialize()
{

}

void UMgbSubsystem::RequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		//호출 실패
		return;
	}

	//호출 됐으나 서버에서 온 결과 처리
	int32 StatusCode = Response->GetResponseCode();
	FString ResponseContent = Response->GetContentAsString();

	UE_LOG(LogTemp, Warning, TEXT("Code : %d, %s"), StatusCode, *(ResponseContent));

	auto JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);

	TSharedPtr<FJsonObject> JsonObject;
	FJsonSerializer::Deserialize(JsonReader, JsonObject);

	auto Name = JsonObject->GetField(TEXT("name"), EJson::String);
	auto Result = JsonObject->GetField(TEXT("result"), EJson::Boolean);

	UE_LOG(LogTemp, Warning, TEXT("name  : %s, result : %d"), *Name->AsString(), Result->AsBool());
}

void UMgbSubsystem::Login()
{
	auto Request = HttpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMgbSubsystem::RequestCompleted);

	FString ID = "admin";
	FString Passwd = "1234";
	FString URL = FString::Printf(TEXT("http://127.0.0.1:8080/api/login?user_id=%s&passwd=%s"), *ID, *Passwd);
	
	Request->SetURL(URL);
	Request->SetVerb(TEXT("GET"));

	Request->ProcessRequest();
}