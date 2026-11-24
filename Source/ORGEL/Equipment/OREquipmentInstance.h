#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "UObject/Object.h"
#include "AbilitySystem/ORAbilitySet.h"
#include "OREquipmentInstance.generated.h"

class APawn;
class UOREquipmentData;
class UORAbilitySystemComponent;

UCLASS()
class ORGEL_API UOREquipmentInstance : public UObject
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Equipment Data
// ─────────────────────────────────────────────────────────────
public:
	/** 이 인스턴스의 원본 장비 데이터를 지정합니다. */
	void SetEquipmentData(const UOREquipmentData* InEquipmentData) { EquipmentData = InEquipmentData; }

	/** 이 인스턴스의 원본 장비 데이터를 반환합니다. */
	const UOREquipmentData* GetEquipmentData() const { return EquipmentData; }


// ─────────────────────────────────────────────────────────────
// Ability System
// ─────────────────────────────────────────────────────────────
public:
	/** 장비 데이터의 어빌리티와 지속 효과를 부여하고 회수용 핸들을 기록합니다. */
	void GiveToAbilitySystem(UORAbilitySystemComponent* ASC);

	/** 기록한 어빌리티와 효과를 ASC에서 회수하고 핸들을 비웁니다. */
	void TakeFromAbilitySystem(UORAbilitySystemComponent* ASC);


// ─────────────────────────────────────────────────────────────
// Pawn
// ─────────────────────────────────────────────────────────────
public:
	/** 외형을 표시할 Pawn을 지정하며 nullptr이면 기존 외형을 제거합니다. 외형이 없는 장비는 아무것도 하지 않습니다. */
	virtual void SetPawn(APawn* Pawn) {}


// ─────────────────────────────────────────────────────────────
// Equipment Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 이 인스턴스가 생성된 원본 장비 데이터를 참조합니다. */
	UPROPERTY(Transient)
	TObjectPtr<const UOREquipmentData> EquipmentData;

	/** 장착 중 부여한 어빌리티를 해제 시 함께 회수할 수 있도록 핸들을 보관합니다. */
	UPROPERTY(Transient)
	FORAbilitySet_GrantedHandles AbilityHandles;

	/** 장착 중 적용한 효과를 해제 시 함께 제거할 수 있도록 핸들을 보관합니다. */
	UPROPERTY(Transient)
	TArray<FActiveGameplayEffectHandle> EffectHandles;
};
