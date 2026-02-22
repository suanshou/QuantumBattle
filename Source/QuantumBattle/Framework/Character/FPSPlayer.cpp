// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayer.h"

#include "EnhancedInputComponent.h"
#include "Components/SimpleInputComponent.h"
#include "Components/SimpleInputExpandComponent.h"
#include "Components/SimplePlayerItemInterComponent.h"
#include "Config/SimpleActionInputConfig.h"

//防止调试时优化代码
UE_DISABLE_OPTIMIZATION
// Sets default values
AFPSPlayer::AFPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	InteractionComponent = FindComponentByClass<USimplePlayerItemInterComponent>();
}

// Called every frame
void AFPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (USimpleInputComponent* InFPSInputComponent = FindComponentByClass<USimpleInputComponent>())
	{
		InFPSInputComponent->RegisterMappingContext();
	}

	if (USimpleInputExpandComponent* InputExpandComponent = FindComponentByClass<USimpleInputExpandComponent>())
	{
		InputExpandComponent->RegisterMappingContext();
	}
}

// Called to bind functionality to input
void AFPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (USimpleInputComponent* InFPSInputComponent = FindComponentByClass<USimpleInputComponent>())
	{
		InFPSInputComponent->SetupPlayerInputComponent(PlayerInputComponent);
	}

	if (USimpleInputExpandComponent* InputExpandComponent = FindComponentByClass<USimpleInputExpandComponent>())
	{
		InputExpandComponent->SetupPlayerInputComponent(PlayerInputComponent);
	}
}

void AFPSPlayer::K2_OnActionInputTag_Implementation(
	ETriggerEvent InEvent,
	const FInputActionValue& InValue,
	FGameplayTag InputTag)
{
	if (InputTag.GetTagName() == TEXT("PickUp"))
	{
		if (InEvent == ETriggerEvent::Triggered)
		{
			if (InteractionComponent.IsValid())
			{
				InteractionComponent->Trigger(InValue);
			}
		}
	}
	else if (InputTag.GetTagName() == TEXT("Throw"))
	{
		if (InEvent == ETriggerEvent::Triggered)
		{
			if (InteractionComponent.IsValid())
			{
				InteractionComponent->Throw(InValue);
			}
		}
	}
}
UE_ENABLE_OPTIMIZATION