// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimplePlayerItemInterComponent.h"

#include "Actor/Item/Core/SimpleItemActorBase.h"


// Sets default values for this component's properties
USimplePlayerItemInterComponent::USimplePlayerItemInterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SelectItemActorRange = 150.f;
	SphereCenterOffset = FVector::ZeroVector;
	TriggerInterval = 0.f;
	//暂时定一个值，之后再改
	CheckCollisionType = ECollisionChannel::ECC_GameTraceChannel1;
}


// Called when the game starts
void USimplePlayerItemInterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimplePlayerItemInterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//客户端检测，所见即所得
	if (GetOwner() && GetOwner()->HasLocalNetOwner())
	{
		CheckItemAroundPlayer();

		TriggerInterval -= DeltaTime;
		if (TriggerInterval <= 0.f)
		{
			TriggerInterval = 0.f;
		}
	}
}

void USimplePlayerItemInterComponent::Trigger(const FInputActionValue& Value)
{
	if (TriggerInterval <= 0.0f && SelectingItem.IsValid())
	{
		TriggerInterval = 0.5f;

		StartTriggerSelectingItem();
	}
}

void USimplePlayerItemInterComponent::Throw(const FInputActionValue& Value)
{
	if (TriggerInterval <= 0.0f && InteractingItem.IsValid())
	{
		TriggerInterval = 0.5f;

		EndTriggerInteractingItem();
	}
}

void USimplePlayerItemInterComponent::CheckItemAroundPlayer()
{
	if (!IsStartInteraction())
	{
		SelectingItem = nullptr;
		return;
	}

	UWorld* WorldPtr = GetWorld();
	APlayerController* PlayerControllerPtr = WorldPtr ? WorldPtr->GetFirstPlayerController() : nullptr;
	APawn* PlayerCharacterPtr = PlayerControllerPtr ? PlayerControllerPtr->GetPawn() : nullptr;

	//清空缓存
	NearbyItems.Empty();
	SelectingItem = nullptr;

	if (WorldPtr && PlayerControllerPtr && PlayerCharacterPtr)
	{
		FVector PlayerLocation = PlayerCharacterPtr->GetActorLocation();
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{UEngineTypes::ConvertToObjectType(CheckCollisionType)};
		TArray<AActor*> ActorsToIgnore{PlayerCharacterPtr, InteractingItem.Get()};
		TArray<FHitResult> HitResults;
		if (UKismetSystemLibrary::SphereTraceMultiForObjects(
			WorldPtr,
			PlayerLocation + SphereCenterOffset,
			PlayerLocation + SphereCenterOffset,
			SelectItemActorRange,
			ObjectTypes,
			false,
			ActorsToIgnore,
			DrawDebugTraceMode,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			2.f))
		{
			//找到距离视野正前方最近的物体做交互
			double MinItemAngle = 360.0;
			for (auto& HitResult : HitResults)
			{
				if (ASimpleItemActorBase* HitItemActor = Cast<ASimpleItemActorBase>(HitResult.GetActor()))
				{
					if (HitItemActor->IsStartTrigger(this,false))
					{
						NearbyItems.Emplace(HitItemActor);

						FVector OutLocation{};
						FRotator OutRotation{};
						PlayerControllerPtr->GetPlayerViewPoint(OutLocation, OutRotation);

						//OutRotation.Vector()找到当前的前向向量
						double DotValue = FVector::DotProduct(
							(HitResult.GetActor()->GetActorLocation() - OutLocation).GetSafeNormal(),
							OutRotation.Vector().GetSafeNormal());

						double TmpItemAngle = FMath::RadiansToDegrees(FMath::Acos(DotValue));
						if (TmpItemAngle < MinItemAngle)
						{
							MinItemAngle = TmpItemAngle;
							SelectingItem = HitItemActor;
						}
					}
				}
			}
		}
	}
}

void USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem,
	bool bForceInHand)
{
	Super::OnSelectingItemTriggerStart_Implementation(InSelectingItem, bForceInHand);
}

void USimplePlayerItemInterComponent::OnInteractingItemTriggerEnd_Implementation(
	ASimpleItemActorBase* InInteractingItem, bool bIsPutPack)
{
	Super::OnInteractingItemTriggerEnd_Implementation(InInteractingItem, bIsPutPack);
}
