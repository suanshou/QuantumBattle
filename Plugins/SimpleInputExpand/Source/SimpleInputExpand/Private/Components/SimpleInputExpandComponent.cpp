// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleInputExpandComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Interface/SimpleInputInterface.h"


class UEnhancedInputLocalPlayerSubsystem;
// Sets default values for this component's properties
USimpleInputExpandComponent::USimpleInputExpandComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USimpleInputExpandComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleInputExpandComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USimpleInputExpandComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputConfig)
		{
			if (ISimpleInputInterface* OwnerInterface = Cast<ISimpleInputInterface>(PlayerInputComponent->GetOwner()))
			{
				auto InFunc = [OwnerInterface,this](
					const FInputActionValue& Value,
					ETriggerEvent InEvent,
					FGameplayTag InGameplayTag)
				{
					//在FPSPlayer实现
					ISimpleInputInterface::Execute_K2_OnActionInputTag(GetOwner(), InEvent, Value, InGameplayTag);
				};

				auto InBindFunc = [&](
					ETriggerEvent InEvent,
					UInputAction* InInputAction,
					FGameplayTag InGameplayTag)
				{
					//代理参数负载
					EnhancedInputComponent->BindActionValueLambda(
						InInputAction,
						InEvent,
						InFunc,
						InEvent,
						InGameplayTag);
				};
				
				for (const auto& TmpAction:InputConfig->InputActions)
				{
					InBindFunc(ETriggerEvent::Triggered,TmpAction.InputAction,TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Started,TmpAction.InputAction,TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Ongoing,TmpAction.InputAction,TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Canceled,TmpAction.InputAction,TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Completed,TmpAction.InputAction,TmpAction.InputTag);
				}
			}
		}
	}
}

void USimpleInputExpandComponent::RegisterMappingContext()
{
	if (ACharacter* InCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(InCharacter->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}
