// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Config/SimpleActionInputConfig.h"
#include "SimpleInputExpandComponent.generated.h"

class UInputMappingContext;
UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEINPUTEXPAND_API USimpleInputExpandComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="Confug|Input")
	TObjectPtr<USimpleActionInputConfig> InputConfig;

public:
	// Sets default values for this component's properties
	USimpleInputExpandComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	void RegisterMappingContext();
};
