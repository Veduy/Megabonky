// Copyright is owned by Veduy.


#include "DamageTextActor.h"
#include "Components\WidgetComponent.h"
#include "../UI/InGame/DamageText.h"

// Sets default values
ADamageTextActor::ADamageTextActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	SetRootComponent(WidgetComponent);
}

// Called when the game starts or when spawned
void ADamageTextActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.f);
}

void ADamageTextActor::SetText(float InDamage)
{
	UDamageText* Widget = Cast<UDamageText>(WidgetComponent->GetWidget());
	if (Widget)
	{
		Widget->SetDamageText(InDamage);
	}
}
