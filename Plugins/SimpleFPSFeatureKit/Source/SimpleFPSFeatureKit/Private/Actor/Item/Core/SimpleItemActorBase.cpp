// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Core/SimpleItemActorBase.h"


// Sets default values
ASimpleItemActorBase::ASimpleItemActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASimpleItemActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimpleItemActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

