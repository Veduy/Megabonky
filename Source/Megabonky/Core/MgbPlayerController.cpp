// Copyright is owned by Veduy.


#include "MgbPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "MgbGameStateBase.h"
#include "MgbGameModeBase.h"
#include "Characters/MgbPlayerCharacter.h"
#include "Data/WeaponInfo.h"
#include "../UI/InGame/InGame.h"
#include "../Actors/DamageTextActor.h"
#include "../Util/NetworkLog.h"

AMgbPlayerController::AMgbPlayerController()
{
	ConstructorHelpers::FClassFinder<AActor>BP_DamageTextActor(TEXT("/Game/Blueprints/Actors/BP_DamageTextActor.BP_DamageTextActor_C"));
	if (BP_DamageTextActor.Succeeded())
	{
		DamageTextActorClass = BP_DamageTextActor.Class;
	}
}

void AMgbPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);


}

void AMgbPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AMgbPlayerController::OnPossess(APawn* aPawn)
{
	// 오직 서버에서만 호출됨.
	Super::OnPossess(aPawn);
	
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
	AMgbPlayerCharacter* PlayerCharacter = Cast<AMgbPlayerCharacter>(aPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->EquipWeapon(PlayerCharacter->DefaultWeaponClass);
		PlayerCharacter->ActivateWeaponsAbility();
	}
}

void AMgbPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMgbPlayerController::ServerResumeRequestCountIncrementAndCheck_Implementation()
{
	AMgbGameStateBase* GS = Cast<AMgbGameStateBase>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->HandleResumeRequest();
	}
}

void AMgbPlayerController::ClientSpawnDamageTextActor_Implementation(FVector Location, float DamageValue)
{
	FTransform SpawnTransform = FTransform(FRotator(), Location, FVector::OneVector);
	AActor* Actor = GetWorld()->SpawnActor<AActor>(DamageTextActorClass, SpawnTransform);

	if (Actor)
	{
		Cast<ADamageTextActor>(Actor)->SetText(DamageValue);
	}
}

void AMgbPlayerController::GenerateUpgradeInfo()
{
	// 서버에서 호출되야함.
	// 전체 반복문으로 3개 생성해서 전달해야함.

	AMgbGameStateBase* GS = Cast<AMgbGameStateBase>(GetWorld()->GetGameState());
	AMgbPlayerCharacter* PlayerCharacter = Cast<AMgbPlayerCharacter>(GetPawn());

	PlayerCharacter->Weapons;
	PlayerCharacter->Tomes;

	// 2분의 1확률로 무기/비전서.
	bool bWeapon = FMath::RandBool();

	if (bWeapon)
	{
		//무기 칸이 남아있다면 데이터테이블에 있는 무기들중에서 랜덤으로 선택. >> (가지고있는 무기에 추가 확률 부여)
		//무기 칸이 없다면, 플레이어가 가지고 있는 무기들 중에서 선택.
		TArray<FName> WeaponNames;
		WeaponNames = GS->DT_Weapon->GetRowNames();

		int RandomNum = FMath::RandRange(0, WeaponNames.Num() - 1);
		FName RandomWeaponName = WeaponNames[RandomNum];

		// 업그레이드할 무기는 정해짐.
		FMgbWeaponInfo* WeaponInfo = GS->DT_Weapon->FindRow<FMgbWeaponInfo>(RandomWeaponName, FString("Find Weapon"));
		if (!WeaponInfo)
		{
			return;
		}

		auto WeaponBonus = GS->DT_WeaponUpgradeBonus->FindRow<FMgbWeaponUpgradeBonus>(RandomWeaponName, FString("Find Bouns"));

		// 1~2개 선택
		if (!WeaponBonus)
		{
			return;
		}

		auto Temp = WeaponBonus->UpgradeOptions;
		TArray<FWeaponUpgradeOption> SelectedOptions;

		// 섞기
		Temp.Sort([](const auto&, const auto&)
		{
			return FMath::RandBool();
		});

		int32 Count = FMath::RandRange(1, 2);

		for (int32 i = 0; i < Count && i < Temp.Num(); ++i)
		{
			SelectedOptions.Add(Temp[i]);
		}
		
		// UI에 전달할 정보가.
		// 아이템 이름만 전달해주면, UI Widget에서 데이터테이블 보고 정보 알수 있으니까 이름만 전달하면 될거같고, 
		// UpgradeBonus에 대한 정보만 더 전달을 해주면 될거 같은데, 
		InGameWidget->SetItemUpgradeSlot(0, RandomWeaponName, SelectedOptions);
	}
	else
	{

	}

}