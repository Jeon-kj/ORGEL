#pragma once

#include "CoreMinimal.h"
#include "ORAttributeSet.h"
#include "ORCombatSet.generated.h"

UCLASS()
class ORGEL_API UORCombatSet : public UORAttributeSet
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Initialization
// ─────────────────────────────────────────────────────────────
public:
	/** 공격력과 공격속도 배율을 효과가 없는 기본값 1로 설정합니다. */
	UORCombatSet();

	/** 명시적인 초기화에서 공격력과 공격속도 배율을 기본값 1로 되돌립니다. */
	virtual void InitializeSet(const UORCharacterStatData& StatData) override;


// ─────────────────────────────────────────────────────────────
// Attribute Accessors
// ─────────────────────────────────────────────────────────────
public:
	/** 공격력 배율의 GAS 속성 접근 함수를 생성합니다. */
	OR_ATTRIBUTE_ACCESSORS(UORCombatSet, AttackPower);

	/** 공격속도 배율의 GAS 속성 접근 함수를 생성합니다. */
	OR_ATTRIBUTE_ACCESSORS(UORCombatSet, AttackSpeed);


// ─────────────────────────────────────────────────────────────
// Attribute Clamp
// ─────────────────────────────────────────────────────────────
protected:
	/** 공격력은 0 이상, 공격속도는 연사 간격을 계산할 수 있는 양수로 보정합니다. */
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;


// ─────────────────────────────────────────────────────────────
// Combat Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 무기 기본 피해량에 곱할 공격력 배율을 관리합니다. */
	UPROPERTY(BlueprintReadOnly, Category = "변수|전투", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;

	/** 무기 기본 공격속도에 곱할 공격속도 배율을 관리합니다. */
	UPROPERTY(BlueprintReadOnly, Category = "변수|전투", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackSpeed;
};
