// Copyright is owned by Veduy.


#include "MgbGameStateBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

#include "GameFramework/Character.h"
#include "Characters/MgbEnemyCharacter.h"
#include "MgbPlayerController.h"
#include "Data/WeaponInfo.h"
#include "Data/MgbSaveGame.h"

#include "../MgbSubsystem.h"
#include "../Util/NetworkLog.h"
#include "../UI/InGame/InGame.h"

AMgbGameStateBase::AMgbGameStateBase()
{
	SetReplicates(true);
}

void AMgbGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	//진짜 게임이 시작됫을떄 시간으로 나중에 설정(맵 이동 컷신 끝난후로)
	if (HasAuthority())
	{
		GameStartTime = GetServerWorldTimeSeconds();
	}

	// 몬스터 랜덤 순서 섞기.
	EnemyClasses.Sort([](const auto&, const auto&)
		{
			return FMath::RandBool();
		});
}

void AMgbGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ElapsedTime < 1.f)
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime >= 1.f)
		{
			PassedTimeSec = (int)(GetServerWorldTimeSeconds() - GameStartTime);
			OnTimeChanged.Broadcast(PassedTimeSec);
			ElapsedTime = 0.f;

			if(PassedTimeSec % 60 == 0)
			{
				// 60초마다 30초간 몬스터 스폰 증가.
				ActivateSpawnMultiplierBurst(5);
			}
		}
	}
}

void AMgbGameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::EndPlay(EndPlayReason);
}

void AMgbGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMgbGameStateBase, bGameOver);
	DOREPLIFETIME(AMgbGameStateBase, GameStartTime);
	DOREPLIFETIME(AMgbGameStateBase, GameTimeSec);
	DOREPLIFETIME(AMgbGameStateBase, CurrentLevel);
	DOREPLIFETIME(AMgbGameStateBase, Gold);
	DOREPLIFETIME(AMgbGameStateBase, TotalKill);
	DOREPLIFETIME(AMgbGameStateBase, NextBoxGold);
}

void AMgbGameStateBase::ServerAddXP_Implementation(float InValue)
{
	CurrentXP += InValue;
	float Percent = CurrentXP / RequiredXP;

	MulticastUpdateUI_XP(Percent);

	if (CurrentXP >= RequiredXP)
	{
		float temp = CurrentXP - RequiredXP;
		CurrentXP = 0;
		CurrentLevel++;
		RequiredXP = RequiredXP * (1.2f);

		ServerAddXP(temp);

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0001f);

		// 서버에서 각 플레이어마다 업그레이드 옵션을 생성하여 해당 클라이언트로 전송
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (AMgbPlayerController* MgbPC = Cast<AMgbPlayerController>(Iterator->Get()))
			{
				TArray<FUpgradeSlotInfo> Slots = MgbPC->ServerGenerateUpgradeSlots();
				MgbPC->ClientShowUpgradeWindow(Slots);
			}
		}

		MulticastSetPauseGame(true);
	}
}

void AMgbGameStateBase::MulticastUpdateUI_XP_Implementation(float InPercent)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->IsLocalPlayerController())
	{
		AMgbPlayerController* MgbPC = Cast<AMgbPlayerController>(PC);
		if (MgbPC)
		{
			if (MgbPC->InGameWidget)
			{
				MgbPC->InGameWidget->SetXPBarPercent(InPercent);
			}
		}
	}
}

void AMgbGameStateBase::MulticastSetPauseGame_Implementation(bool bPause)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->IsLocalPlayerController())
	{
		PC->SetIgnoreLookInput(bPause);
		PC->SetIgnoreMoveInput(bPause);
	}
	if (bPause)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0001f);
		Cast<AMgbPlayerController>(PC)->Audio_Levelup->Play();
	}
	else
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		Cast<AMgbPlayerController>(PC)->Audio_Levelup->FadeOut(0.3f, 0.f);
	}
}

void AMgbGameStateBase::MulticastShowItemSelectWindow_Implementation()
{
	// 업그레이드 창 표시는 ClientShowUpgradeWindow에서 처리됨
}

void AMgbGameStateBase::MulticastHandleGameOver_Implementation()
{
	HandleGameOver();
}

void AMgbGameStateBase::ServerHandleGameOver_Implementation()
{
	HandleGameOver();
}

void AMgbGameStateBase::ActivateSpawnMultiplierBurst(int32 InValue)
{
	if (!HasAuthority())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Activate Spawn Multiply - '%d'"), InValue);

	GetWorld()->GetTimerManager().ClearTimer(SpawnMultiplierResetTimerHandle);

	EnemySpawnMultiplier = InValue;

	GetWorld()->GetTimerManager().SetTimer(
		SpawnMultiplierResetTimerHandle,
		[this]()
		{
			EnemySpawnMultiplier = 1;
			UE_LOG(LogTemp, Warning, TEXT("Deactivate Spawn Multiply"));
		},
		30.f,
		false
	);
}

void AMgbGameStateBase::InitSpawnEnemyTimer()
{
	if (EnemyClasses.IsEmpty())
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		[this]()
		{
			SpawnEnemy();   
		},
		SpawnIntervalTime,
		true,
		1.f
	);
}


void AMgbGameStateBase::SpawnEnemy()
{
	// Only Server
	if (!HasAuthority())
	{
		return;
	}

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	//TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForTrace;
	//ObjectTypesForTrace.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	// 플레이어 수만큼 플레이어를 타겟으로한 몬스터를 소환한다.
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Get())
		{
			APawn* Player = Iterator->Get()->GetPawn();
			if (!Player)
			{
				return;
			}
		
			// 원형태의 랜덤 방향에서 스폰
			float y = sinf(rand());
			float x = cosf(rand());
			FVector2D Dir = FVector2D(x, y);

			// 일정 Min~Max범위의 값을 곱함 -> 스폰 범위 지정.
			FVector2D Location = Dir * FMath::FRandRange(SpawnRange - 300.f, SpawnRange + 300.f);
			
			// 스폰할 EnemyClass 선택	(1분마다 종류증가)
			uint8 EnemyClassVariety = 0;
			if (EnemyClasses.Num() > 1)
			{
				EnemyClassVariety = (PassedTimeSec / 60) % EnemyClasses.Num();
			}
			UClass* EnemyClass = EnemyClasses[FMath::RandRange(0, EnemyClassVariety)];

			
			if (PassedTimeSec == 0)
				return;

			// 스폰할 마리수 계산 
			int EnemyToSpawn = EnemySpawnMultiplier * FMath::Loge(float(1 + (PassedTimeSec * PassedTimeSec) / 600)) + 3;
			EnemyToSpawn = FMath::Clamp(EnemyToSpawn, 0, 100);

			// 스폰할 지점에서 또 작은 원을 기준으로 스폰할 마리수에 해당하는, 진짜 스폰 지점을 뽑아내서 그 지점에 스폰.
			for (int i = 0; i < EnemyToSpawn; ++i)
			{
				if (CurrentEnemyCount >= MaxEnemyCount)
					return;

				FVector2D SmallDir = FVector2D(cosf(rand()), sinf(rand()));
				FVector2D SmallLocation = SmallDir * FMath::FRandRange(500.f, 1000.f);

				// 플레이어 위치(x,y 좌표만 가져와야함).
				FVector2D PlayerXY = FVector2D(Player->GetActorLocation().X, Player->GetActorLocation().Y);

				FVector2D RealSpawn2D = Location + SmallLocation + PlayerXY;

				FVector RealSpawnLocation = FVector(RealSpawn2D.X, RealSpawn2D.Y, 0);

				// 스폰할 높이는 LineTrace로 설정.
				FVector Start = RealSpawnLocation + FVector(0.f, 0.f, 50000.f);
				FVector End = RealSpawnLocation + FVector(0.f, 0.f, -50000.f);

				FHitResult Hit;

				// Use engine's collision query params to only get the hit result (location)
				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(Player);

				bool bResult = GetWorld()->LineTraceSingleByObjectType(
					Hit,
					Start,
					End,
					ObjectQueryParams,
					QueryParams
				);

				if (!bResult)
				{
					continue;
				}

				// 스폰
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(Hit.Location, Player->GetActorLocation());
				SpawnRotation.Pitch = 0.f;
				SpawnRotation.Roll = 0.f;
				
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemyClass, Hit.Location, SpawnRotation, Params);
				AMgbEnemyCharacter* Enemy = Cast<AMgbEnemyCharacter>(SpawnedActor);

				if (Enemy)
				{
					Enemy->TargetActor = Player;
					CurrentEnemyCount++;
				}
			}
		}
	}
}

void AMgbGameStateBase::HandleGameOver()
{
	if (bGameOver)
	{
		return;
	}

	bGameOver = true;

	// 게임 세션 기록 수집 및 저장 (서버에서만)
	if (HasAuthority())
	{
		FGameSessionRecord SessionRecord;
		SessionRecord.TotalKills = TotalKill;
		SessionRecord.GoldEarned = Gold;

		// 무기별 킬 통계 수집
		for (const auto& Pair : WeaponKillRecord)
		{
			SessionRecord.WeaponKills.Add(FWeaponKillRecord(Pair.Key, Pair.Value));
		}

		// Subsystem을 통해 저장
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UMgbSubsystem* Subsystem = GI->GetSubsystem<UMgbSubsystem>())
			{
				Subsystem->SaveGameSession(SessionRecord);
			}
		}
	}

	OnGameOver.Broadcast();
	if (SpawnTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

void AMgbGameStateBase::RecordWeaponKill(FName WeaponName)
{
	if (WeaponName.IsNone())
	{
		WeaponName = FName("Unknown");  // 무기 정보가 없으면 "Unknown"으로 기록
	}

	if (int64* KillCount = WeaponKillRecord.Find(WeaponName))
	{
		(*KillCount)++;
	}
	else
	{
		WeaponKillRecord.Add(WeaponName, 1);
	}

	UE_LOG(LogTemp, Log, TEXT("Weapon Kill: %s (Total: %lld)"),
		*WeaponName.ToString(), WeaponKillRecord[WeaponName]);
}

void AMgbGameStateBase::HandleResumeRequest()
{
	ResumeRequestCount++;
	
	if (ResumeRequestCount == GetCurrentPlayerCount())
	{	
		ResumeRequestCount = 0;

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f); //서버도 똑같이
		MulticastSetPauseGame(false);
	}
}

uint32 AMgbGameStateBase::GetCurrentPlayerCount()
{
	uint32 Count = 0;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		Count++;
	}
	return Count;
}