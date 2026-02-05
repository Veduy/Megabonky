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
		auto Name = JsonObject->GetField(TEXT("name"), EJson::String);
		UE_LOG(LogTemp, Warning, TEXT("Login Success - name: %s"), *Name->AsString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Failed"));
	}
}

void UMgbSubsystem::Login()
{
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