#include "OREquipmentInstance.h"

#include "GameplayEffect.h"
#include "OREquipmentData.h"
#include "AbilitySystem/ORAbilitySystemComponent.h"

void UOREquipmentInstance::GiveToAbilitySystem(UORAbilitySystemComponent* ASC)
{
	// 어빌리티의 출처를 인스턴스로 기록하여 어빌리티가 장비 데이터와 외형에 접근할 수 있도록 합니다.
	if (const UORAbilitySet* AbilitySet = EquipmentData->GetAbilitySet())
	{
		AbilitySet->GiveToAbilitySystem(ASC, &AbilityHandles, this);
	}

	// 효과의 출처도 인스턴스로 기록하여 이후 피해와 버프 계산에서 장비를 구분할 수 있도록 합니다.
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

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
			EffectHandles.Add(Handle);
		}
	}
}

void UOREquipmentInstance::TakeFromAbilitySystem(UORAbilitySystemComponent* ASC)
{
	// 장비가 부여한 어빌리티를 회수합니다.
	AbilityHandles.TakeFromAbilitySystem(ASC);

	// 회수 콜백이 재진입해도 같은 효과를 다시 제거하지 않도록 핸들을 먼저 이동한 뒤 제거합니다.
	const TArray<FActiveGameplayEffectHandle> Handles = MoveTemp(EffectHandles);
	for (const FActiveGameplayEffectHandle& Handle : Handles)
	{
		ASC->RemoveActiveGameplayEffect(Handle);
	}
}
