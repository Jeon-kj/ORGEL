#include "OREquipmentComponent.h"

#include "AbilitySystemGlobals.h"
#include "OREquipmentInstance.h"
#include "OREquipmentPickup.h"
#include "AbilitySystem/ORAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"

void UOREquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// 장착 이후 Pawn이 생성되거나 교체되어도 장비 외형이 따라가도록 Pawn 변경 이벤트를 등록합니다.
	CastChecked<APlayerState>(GetOwner())->OnPawnSet.AddDynamic(this, &ThisClass::HandlePawnSet);
}

void UOREquipmentComponent::Equip(const UOREquipmentData* EquipmentData)
{
	// 장비 종류에 맞는 인스턴스를 생성하고 원본 데이터를 연결한 뒤 장착합니다.
	UOREquipmentInstance* EquipmentInstance = NewObject<UOREquipmentInstance>(GetOwner(), EquipmentData->GetInstanceClass());
	EquipmentInstance->SetEquipmentData(EquipmentData);
	Equip(EquipmentInstance);
}

void UOREquipmentComponent::Equip(UOREquipmentInstance* EquipmentInstance)
{
	// 같은 슬롯에 장착된 기존 장비를 먼저 해제하여 바닥에 떨어뜨립니다.
	const EOREquipmentSlot Slot = EquipmentInstance->GetEquipmentData()->GetSlot();
	Unequip(Slot);

	// 어빌리티와 효과를 부여하고 현재 Pawn에 외형을 표시하며, Pawn이 아직 없으면 Pawn 변경 이벤트에서 표시합니다.
	EquipmentInstance->GiveToAbilitySystem(GetAbilitySystemComponent());
	EquipmentInstance->SetPawn(CastChecked<APlayerState>(GetOwner())->GetPawn());

	// 장착을 마친 인스턴스를 슬롯에 등록합니다.
	EquippedInstances.Add(Slot, EquipmentInstance);
}

void UOREquipmentComponent::Unequip(EOREquipmentSlot Slot)
{
	// 회수 콜백이 재진입해도 같은 장비를 다시 처리하지 않도록 슬롯에서 먼저 꺼내며, 빈 슬롯은 처리하지 않습니다.
	TObjectPtr<UOREquipmentInstance> EquipmentInstance;
	if (!EquippedInstances.RemoveAndCopyValue(Slot, EquipmentInstance))
	{
		return;
	}

	// 장비가 표시한 외형을 제거하고 부여한 어빌리티와 효과를 회수합니다.
	EquipmentInstance->SetPawn(nullptr);
	EquipmentInstance->TakeFromAbilitySystem(GetAbilitySystemComponent());

	// 떨어뜨릴 위치를 정할 Pawn이 없으면 장비를 버립니다.
	const ACharacter* Character = CastChecked<APlayerState>(GetOwner())->GetPawn<ACharacter>();
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn이 없어 해제한 장비를 떨어뜨리지 못했습니다: %s"), *EquipmentInstance->GetEquipmentData()->GetName());
		return;
	}

	// 캡슐 바닥 위치에 픽업을 생성하고 해제한 인스턴스를 넘겨 장비 상태를 유지합니다.
	const FVector DropLocation = Character->GetActorLocation() - FVector(0.0f, 0.0f, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	AOREquipmentPickup* Pickup = GetWorld()->SpawnActor<AOREquipmentPickup>(DropLocation, FRotator::ZeroRotator);
	Pickup->SetEquipmentInstance(EquipmentInstance);
}

UORAbilitySystemComponent* UOREquipmentComponent::GetAbilitySystemComponent() const
{
	return CastChecked<UORAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()));
}

void UOREquipmentComponent::HandlePawnSet(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
	// 장착 중인 모든 장비의 외형을 새 Pawn으로 옮기며, 외형이 없는 장비는 아무것도 하지 않습니다.
	for (const TPair<EOREquipmentSlot, TObjectPtr<UOREquipmentInstance>>& Pair : EquippedInstances)
	{
		Pair.Value->SetPawn(NewPawn);
	}
}
