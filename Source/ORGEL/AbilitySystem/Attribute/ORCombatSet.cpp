#include "ORCombatSet.h"

#include "AbilitySystem/ORAbilitySystemComponent.h"

UORCombatSet::UORCombatSet()
	: AttackPower(1.0f)
	, AttackSpeed(1.0f)
{
}

void UORCombatSet::InitializeSet(const UORCharacterStatData& StatData)
{
	// 장비와 효과가 없는 기본 상태로 두 배율을 초기화합니다.
	UORAbilitySystemComponent* ASC = GetORAbilitySystemComponent();
	ASC->SetNumericAttributeBase(GetAttackPowerAttribute(), 1.0f);
	ASC->SetNumericAttributeBase(GetAttackSpeedAttribute(), 1.0f);
}

void UORCombatSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	// 음수 공격력과 유효하지 않은 수치가 피해 계산에 전달되지 않도록 처리합니다.
	if (Attribute == GetAttackPowerAttribute())
	{
		NewValue = FMath::IsFinite(NewValue) ? FMath::Max(NewValue, 0.0f) : 0.0f;
	}
	// 연사 간격을 공격속도로 나누므로 0 이하로 내려가지 않도록 최소값을 보장합니다.
	else if (Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::IsFinite(NewValue) ? FMath::Max(NewValue, 0.01f) : 1.0f;
	}
}
