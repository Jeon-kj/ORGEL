#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OREquipmentData.h"
#include "OREquipmentComponent.generated.h"

class APawn;
class APlayerState;
class UOREquipmentInstance;
class UORAbilitySystemComponent;

UCLASS()
class ORGEL_API UOREquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Lifecycle
// ─────────────────────────────────────────────────────────────
private:
	/** 소유 플레이어 상태의 Pawn 변경 이벤트를 등록합니다. */
	virtual void BeginPlay() override;


// ─────────────────────────────────────────────────────────────
// Equipment
// ─────────────────────────────────────────────────────────────
public:
	/** 장비 데이터로 새 인스턴스를 생성하여 장착합니다. */
	void Equip(const UOREquipmentData* EquipmentData);

	/** 같은 슬롯의 기존 장비를 바닥에 떨어뜨린 뒤, 상태를 유지한 인스턴스를 그대로 장착합니다. */
	void Equip(UOREquipmentInstance* EquipmentInstance);

	/** 지정한 슬롯의 장비가 부여한 어빌리티와 효과 및 외형을 회수하고 Pawn 발밑에 떨어뜨립니다. */
	void Unequip(EOREquipmentSlot Slot);

private:
	/** 소유자가 제공하는 ASC를 프로젝트 타입으로 반환합니다. */
	UORAbilitySystemComponent* GetAbilitySystemComponent() const;


// ─────────────────────────────────────────────────────────────
// Pawn
// ─────────────────────────────────────────────────────────────
private:
	/** Pawn이 생성되거나 교체되면 장착 중인 장비의 외형을 새 Pawn으로 옮깁니다. */
	UFUNCTION()
	void HandlePawnSet(APlayerState* Player, APawn* NewPawn, APawn* OldPawn);


// ─────────────────────────────────────────────────────────────
// Equipment Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 슬롯별로 장착 중인 장비 인스턴스를 보관합니다. */
	UPROPERTY(Transient)
	TMap<EOREquipmentSlot, TObjectPtr<UOREquipmentInstance>> EquippedInstances;
};
