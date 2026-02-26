// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Pickable/Core/SimpleItemActorPickable.h"
#include "SimpleItemActorWeapon.generated.h"

class USimpleWeaponInstance;

UCLASS()
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorWeapon : public ASimpleItemActorPickable
{
	GENERATED_BODY()

protected:
	//武器模型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	//武器实例类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<USimpleWeaponInstance> WeaponInstanceClass;

private:
	//网络同步的武器实例
	UPROPERTY(Replicated)
	TObjectPtr<USimpleWeaponInstance> WeaponInstance;

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Sets default values for this actor's properties
	ASimpleItemActorWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand) override;
	virtual void OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack) override;

	//和蓝图联动
public:
	UFUNCTION(BlueprintPure, Category="Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	UFUNCTION(BlueprintPure, Category="Weapon")
	USimpleWeaponInstance* GetWeaponInstance() const { return WeaponInstance; }

	UFUNCTION(BlueprintCallable,Category="Weapon")
	void EquipToSlot();

	UFUNCTION(BlueprintCallable,Category="Weapon")
	void EquipToHand();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
