#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Components/ActorComponent.h"
#include "AbilitySystem/ORAbilitySet.h"
#include "OREquipmentData.h"
#include "OREquipmentComponent.generated.h"

class UORAbilitySystemComponent;

USTRUCT()
struct FOREquipmentEntry
{
	GENERATED_BODY()

	/** 어빌리티가 출처로 참조하는 장비 데이터가 장착 중 수거되지 않도록 보관합니다. */
	UPROPERTY(Transient)
	TObjectPtr<UOREquipmentData> EquipmentData;

	/** 이 장비가 부여한 어빌리티를 해제 시 함께 회수할 수 있도록 핸들을 보관합니다. */
	UPROPERTY(Transient)
	FORAbilitySet_GrantedHandles AbilityHandles;

	/** 이 장비가 적용한 효과를 해제 시 함께 제거할 수 있도록 핸들을 보관합니다. */
	UPROPERTY(Transient)
	TArray<FActiveGameplayEffectHandle> EffectHandles;
};

UCLASS()
class ORGEL_API UOREquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Equipment
// ─────────────────────────────────────────────────────────────
public:
	/** 같은 슬롯의 기존 장비를 해제한 뒤 새 장비의 어빌리티와 효과를 부여합니다. */
	void Equip(UOREquipmentData* EquipmentData);

	/** 지정한 슬롯의 장비가 부여한 어빌리티와 효과를 회수하고 슬롯을 비웁니다. */
	void Unequip(EOREquipmentSlot Slot);

private:
	/** 소유자가 제공하는 ASC를 프로젝트 타입으로 반환합니다. */
	UORAbilitySystemComponent* GetAbilitySystemComponent() const;


// ─────────────────────────────────────────────────────────────
// Equipment Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 슬롯별로 장착 중인 장비와 회수용 핸들을 보관합니다. */
	UPROPERTY(Transient)
	TMap<EOREquipmentSlot, FOREquipmentEntry> EquippedEntries;
};
