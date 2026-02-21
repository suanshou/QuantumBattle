// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
USimpleInputComponent::USimpleInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USimpleInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USimpleInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USimpleInputComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//动态加载输入组件
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&USimpleInputComponent::Move);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&USimpleInputComponent::Look);

		if (ACharacter* InCharacter = Cast<ACharacter>(GetOwner()))
		{
			EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,InCharacter,&ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,InCharacter,&ACharacter::StopJumping);
		}
	}
}

void USimpleInputComponent::RegisterMappingContext()
{
	if (ACharacter* InCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(InCharacter->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext,0);
			}
		}
	}
}

void USimpleInputComponent::Move(const FInputActionValue& Value)
{
	if (ACharacter* InCharacter = Cast<ACharacter>(GetOwner()))
	{
		FVector2D MoveVector = Value.Get<FVector2D>();
		if (InCharacter->Controller != nullptr)
		{
			InCharacter->AddMovementInput(InCharacter->GetActorRightVector(),MoveVector.X);
			InCharacter->AddMovementInput(InCharacter->GetActorForwardVector(),MoveVector.Y);
		}
	}
}

void USimpleInputComponent::Look(const FInputActionValue& Value)
{
	if (ACharacter* InCharacter = Cast<ACharacter>(GetOwner()))
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();
		if (InCharacter->Controller != nullptr)
		{
			InCharacter->AddControllerYawInput(LookAxisVector.X);
			InCharacter->AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

