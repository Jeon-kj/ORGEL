#include "OREquipmentComponent.h"

#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "AbilitySystem/ORAbilitySystemComponent.h"

void UOREquipmentComponent::Equip(UOREquipmentData* EquipmentData)
{
	// 같은 슬롯에 장착된 기존 장비의 어빌리티와 효과를 먼저 회수합니다.
	Unequip(EquipmentData->GetSlot());

	UORAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	FOREquipmentEntry Entry;
	Entry.EquipmentData = EquipmentData;

	// 어빌리티 세트를 지정한 장비만 어빌리티를 부여하고 회수용 핸들을 기록합니다.
	if (const UORAbilitySet* AbilitySet = EquipmentData->GetAbilitySet())
	{
		AbilitySet->GiveToAbilitySystem(ASC, &Entry.AbilityHandles, EquipmentData);
	}

	// 효과의 출처를 장비 데이터로 기록하여 이후 피해와 버프 계산에서 구분할 수 있도록 합니다.
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(EquipmentData);

	// 설정된 효과를 자신에게 적용하고 실제로 활성화된 효과만 회수 목록에 등록합니다.
	for (const TSubclassOf<UGameplayEffect>& EffectClass : EquipmentData->GetGameplayEffects())
	{
		if (!EffectClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("장비 데이터의 비어 있는 효과 항목을 건너뜁니다: %s"), *EquipmentData->GetName());
			continue;
		}

		const FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectToSelf(EffectClass.GetDefaultObject(), 1.0f, EffectContext);
		if (Handle.IsValid())
		{
			Entry.EffectHandles.Add(Handle);
		}
	}

	// 부여를 마친 장비를 슬롯에 등록합니다.
	EquippedEntries.Add(EquipmentData->GetSlot(), MoveTemp(Entry));
}

void UOREquipmentComponent::Unequip(EOREquipmentSlot Slot)
{
	// 회수 콜백이 재진입해도 같은 장비를 다시 처리하지 않도록 슬롯에서 먼저 꺼내며, 빈 슬롯은 처리하지 않습니다.
	FOREquipmentEntry Entry;
	if (!EquippedEntries.RemoveAndCopyValue(Slot, Entry))
	{
		return;
	}

	// 장비가 부여한 어빌리티를 회수하고 적용한 효과를 모두 제거합니다.
	UORAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	Entry.AbilityHandles.TakeFromAbilitySystem(ASC);
	for (const FActiveGameplayEffectHandle& Handle : Entry.EffectHandles)
	{
		ASC->RemoveActiveGameplayEffect(Handle);
	}
}

UORAbilitySystemComponent* UOREquipmentComponent::GetAbilitySystemComponent() const
{
	return CastChecked<UORAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()));
}
