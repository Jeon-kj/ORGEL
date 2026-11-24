#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OREquipmentData.generated.h"

class UGameplayEffect;
class UORAbilitySet;
class UOREquipmentInstance;
class USkeletalMesh;

UENUM(BlueprintType)
enum class EOREquipmentSlot : uint8
{
	Weapon,
	Armor,
	Shoes,
	Effector
};

UCLASS()
class ORGEL_API UOREquipmentData : public UDataAsset
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Equipment
// ─────────────────────────────────────────────────────────────
public:
	/** 이 장비를 장착할 슬롯을 반환합니다. */
	EOREquipmentSlot GetSlot() const { return Slot; }

	/** 장착 중 부여할 어빌리티 세트를 반환하며, 지정하지 않은 장비는 nullptr를 반환합니다. */
	const UORAbilitySet* GetAbilitySet() const { return AbilitySet; }

	/** 장착 중 유지할 게임플레이 효과 클래스 목록을 반환합니다. */
	const TArray<TSubclassOf<UGameplayEffect>>& GetGameplayEffects() const { return GameplayEffects; }

	/** 바닥에 떨어진 장비와 장착한 무기에 표시할 외형 메시를 반환합니다. */
	USkeletalMesh* GetMesh() const { return Mesh; }


// ─────────────────────────────────────────────────────────────
// Instance
// ─────────────────────────────────────────────────────────────
public:
	/** 이 장비를 장착할 때 생성할 인스턴스 클래스를 반환합니다. */
	virtual TSubclassOf<UOREquipmentInstance> GetInstanceClass() const;


// ─────────────────────────────────────────────────────────────
// Slot Variables
// ─────────────────────────────────────────────────────────────
protected:
	/** 이 장비가 차지할 슬롯을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|장비")
	EOREquipmentSlot Slot = EOREquipmentSlot::Weapon;


// ─────────────────────────────────────────────────────────────
// Equipment Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 장착 시 부여하고 해제 시 회수할 어빌리티 세트를 지정하며, 효과만 사용하는 장비는 비워 둡니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|장비")
	TObjectPtr<UORAbilitySet> AbilitySet;

	/** 장착 시 자신에게 적용하고 해제 시 제거할 지속형 효과 클래스를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|장비")
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffects;

	/** 바닥에 떨어졌을 때 표시하고, 무기는 장착 시 손에도 부착할 외형 메시를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|외형")
	TObjectPtr<USkeletalMesh> Mesh;
};
