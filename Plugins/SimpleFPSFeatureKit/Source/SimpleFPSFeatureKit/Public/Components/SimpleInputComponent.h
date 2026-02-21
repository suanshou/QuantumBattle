// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleInputComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class UInputComponent;
struct FInputActionValue;
//可以以变量形式访问，可以被蓝图继承，可以在蓝图列表内生成
UCLASS(BlueprintType,Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimpleInputComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

public:
	// Sets default values for this component's properties
	USimpleInputComponent();

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
	
public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
