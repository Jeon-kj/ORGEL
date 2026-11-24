#pragma once

#include "CoreMinimal.h"
#include "Character/OGCharacter.h"
#include "OGEnemy.generated.h"

class UORAbilitySet;
class UORContactDamageComponent;
class UORMovementSet;
class UORCharacterStatData;
class UORHealthSet;

UCLASS()
class ORGEL_API AOGEnemy : public AOGCharacter
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Lifecycle
// ─────────────────────────────────────────────────────────────
public:
	/** 몬스터가 소유할 ASC와 체력 속성 세트를 생성합니다. */
	AOGEnemy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/** 몬스터 최초 생성 시 ASC를 연결하고 체력을 초기화합니다. */
	virtual void BeginPlay() override;


// ─────────────────────────────────────────────────────────────
// Hit Feedback
// ─────────────────────────────────────────────────────────────
protected:
	/** 공격 등 피격 외 몽타주가 재생 중이면 피격 몽타주를 재생하지 않습니다. */
	virtual bool CanPlayHitMontage() const override;


// ─────────────────────────────────────────────────────────────
// Component
// ─────────────────────────────────────────────────────────────
private:
	/** 몬스터의 현재 체력과 최대 체력을 관리할 속성 세트를 소유합니다. */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UORHealthSet> HealthSet;
	
	/** 몬스터의 기본 이동속도와 이동속도 효과를 관리할 속성 세트를 소유합니다. */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UORMovementSet> MovementSet;

	/** 플레이어와 닿아 있는 동안 접촉 피해를 주는 판정 캡슐을 소유합니다. */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UORContactDamageComponent> ContactDamageComponent;

	
// ─────────────────────────────────────────────────────────────
// Ability System Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 몬스터 최초 생성 시 적용할 필수 초기 스탯 데이터를 참조합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|초기능력치")
	TObjectPtr<UORCharacterStatData> InitialStatData;

	/** 몬스터 최초 생성 시 부여할 공격 어빌리티 묶음을 참조합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|어빌리티")
	TObjectPtr<UORAbilitySet> AbilitySet;
};