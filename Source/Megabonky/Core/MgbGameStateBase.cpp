// Copyright is owned by Veduy.


#include "MgbGameStateBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

#include "Characters/MgbEnemyCharacter.h"
#include "MgbPlayerController.h"

#include "../Util/NetworkLog.h"
#include "../UI/InGame/InGame.h"

AMgbGameStateBase::AMgbGameStateBase()
{
	SetReplicates(true);
}

void AMgbGameStateBase::BeginPlay()
{
	Super::BeginPlay();

}

void AMgbGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}


void AMgbGameStateBase::ServerAddXP_Implementation(float InValue)
{
	CurrentXP += InValue;
	float Percent = CurrentXP / RequiredXP;

	MulticastUpdateUI_XP(Percent);

	// 레벨업 작업
	if (CurrentXP >= RequiredXP)
	{
		float temp = CurrentXP - RequiredXP;
		CurrentXP = 0;
		RequiredXP = RequiredXP * (1.2f);

		ServerAddXP(temp);

		MulticastSetPauseGame(true);
		MulticastShowItemSelectWindow();
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
	UGameplayStatics::SetGamePaused(GetWorld(), bPause);
}

void AMgbGameStateBase::MulticastShowItemSelectWindow_Implementation()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->IsLocalPlayerController())
	{
		AMgbPlayerController* MgbPC = Cast<AMgbPlayerController>(PC);
		if (MgbPC)
		{
			if (MgbPC)
			{
				MgbPC->InGameWidget->ShowItemSelectWindow();
			}
		}
	}
}

void AMgbGameStateBase::InitSpawnEnemyTimer()
{
	if (EnemyClasses.IsEmpty())
	{
		return;
	}

	FTimerHandle SpawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		[this]()
		{
			SpawnEnemy();   
		},
		5.0f,
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

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Get())
		{
			APawn* Player = Iterator->Get()->GetPawn();
			if (!Player)
			{
				return;
			}
		
			// 캐릭터의 좌표를 기준으로 원형태의 랜덤 방향을 구함.
			float y = sinf(rand());
			float x = cosf(rand());
			FVector2D Dir = FVector2D(x, y);

			// 일정 Min~Max범위의 값을 곱함 -> 캐릭터 근처 에서 스폰될 거리 지정.
			FVector2D Location = Dir * FMath::FRandRange(500.f, 1000.f);

			// 스폰할 지점에서 또 작은 원을 기준으로 스폰할 마리수에 해당하는, 진짜 스폰 지점을 뽑아내서 그 지점에 스폰.
			for (int i = 0; i < 5; ++i)
			{
				FVector2D SmallDir = FVector2D(cosf(rand()), sinf(rand()));
				FVector2D SmallLocation = SmallDir * FMath::FRandRange(300.f, 500.f);
				// 플레이어 위치(x,y 좌표만 가져와야함).
				FVector2D PlayerXY = FVector2D(Player->GetActorLocation().X, Player->GetActorLocation().Y);

				FVector2D RealSpawn2D = Location + SmallLocation + PlayerXY;

				FVector RealSpawnLocation = FVector(RealSpawn2D.X, RealSpawn2D.Y, 0);


				// 스폰할 높이는 LineTrace로 설정.
				FVector Start = RealSpawnLocation + FVector(0.f, 0.f, 3000.f);
				FVector End = RealSpawnLocation + FVector(0.f, 0.f, -3000.f);

				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

				TArray<AActor*> ActorsToIgnore;
				FHitResult Hit;

				bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End,
					ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, false, FLinearColor::Red,
					FLinearColor::Green, 2.f);
				
				// 땅밑에서 스폰
				if (bResult)
				{
					float CapsuleHeight = Cast<ACharacter>(Player)->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
					RealSpawnLocation = FVector(Hit.Location.X, Hit.Location.Y, Hit.Location.Z - CapsuleHeight);
				}

				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemyClasses[0], RealSpawnLocation, FRotator::ZeroRotator, Params);
				AMgbEnemyCharacter* Enemy = Cast<AMgbEnemyCharacter>(SpawnedActor);
				if (Enemy)
				{
					Enemy->TargetActor = Player;
				}
			}
		}
	}
}

void AMgbGameStateBase::HandleResumeRequest()
{
	ResumeRequestCount++;
	
	if (ResumeRequestCount == GetCurrentPlayerCount())
	{	
		NET_LOG("");
		ResumeRequestCount = 0;

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