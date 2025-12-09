// Copyright is owned by Veduy.


#include "NetworkLog.h"

#include "../Megabonky.h"

NetworkLog::NetworkLog()
{
}

NetworkLog::~NetworkLog()
{
}

void NetworkLog::Log(UWorld* World, const FString& CallInfo, const FString& InString)
{
	if (!World)
	{
		return;
	}

	FString Prefix;

	switch (World->GetNetMode())
	{
	case NM_Client:
		// GPlayInEditorID 0 is always the server, so 1 will be first client.
		// You want to keep this logic in sync with GeneratePIEViewportWindowTitle and UpdatePlayInEditorWorldDebugString
		Prefix = FString::Printf(TEXT("Client %d: "), UE::GetPlayInEditorID());
		break;
	case NM_DedicatedServer:
	case NM_ListenServer:
		Prefix = FString::Printf(TEXT("Server: "));
		break;
	case NM_Standalone:
		break;
	}

	UE_LOG(LogNetwork, Warning, TEXT("%s %s - %s"), *(Prefix), *CallInfo, *InString)
}
