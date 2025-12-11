// Copyright is owned by Veduy.


#include "MgbCharacter.h"

#include "AbilitySystem/MgbAbilitySystemComponent.h"
#include "Engine/DamageEvents.h"
#include "MgbWeapon.h"

#include "../Util/NetworkLog.h"

// Sets default values
AMgbCharacter::AMgbCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UMgbAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

// Called when the game starts or when spawned
void AMgbCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMgbCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* AMgbCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AMgbCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		
	NET_LOG(FString::Printf(TEXT("DamageCauser : %s"), *DamageCauser->GetName()));			// DamageCauser -> Weapon
	NET_LOG(FString::Printf(TEXT("Weapon : %s"), *DamageCauser->GetName()));	
	NET_LOG(FString::Printf(TEXT("EventInstigator : %s"), *EventInstigator->GetName()));	// PlayerController 

	if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		AMgbWeapon* SourceWeapon = Cast<AMgbWeapon>(DamageCauser);
		AMgbCharacter* SourceCharacter = Cast<AMgbCharacter>(EventInstigator->GetPawn());

		if (SourceWeapon)
		{
			FGameplayEffectContextHandle EffectContextHandle = SourceWeapon->GetAbilitySystemComponent()->MakeEffectContext();
			EffectContextHandle.AddSourceObject(SourceWeapon);
			EffectContextHandle.AddInstigator(SourceWeapon, EventInstigator);

			if (SourceWeapon->DamageEffectClass)
			{
				// Spec을 생성한 컴포넌트가 ExecCalc의 Source로 설정.
				FGameplayEffectSpecHandle EffectSpecHandle = SourceWeapon->GetAbilitySystemComponent()->MakeOutgoingSpec(SourceWeapon->DamageEffectClass, 1.f, EffectContextHandle);

				SourceWeapon->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), AbilitySystemComponent);
			}
		}

		/*AMgbWeapon* Weapon = Cast<AMgbWeapon>(DamageCauser);
		if (Weapon)
		{
			//GameplayEffect 에서 Custom Calculation -> 내부에서 캐릭터, 무기의 Attribute정보로 데미지 계산.
			FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
			EffectContextHandle.AddSourceObject(Weapon); // “어떤 오브젝트가 발생시켰는지” 기록하기 위해
			EffectContextHandle.AddInstigator(DamageCauser, EventInstigator); // GameplayEffect 를 “누가 발생시켰는지” 기록하기 위해
			
			if (Weapon->DamageEffectClass)
			{
				FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Weapon->DamageEffectClass,1.f,EffectContextHandle);
				
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			}
		}*/
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{	

	}

	
	

	return 0.0f;
}

