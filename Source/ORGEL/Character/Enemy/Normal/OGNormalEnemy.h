#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/OGEnemy.h"
#include "OGNormalEnemy.generated.h"

UCLASS()
class ORGEL_API AOGNormalEnemy : public AOGEnemy
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Attack
// ─────────────────────────────────────────────────────────────
public:
	/** 플레이어와의 수평 거리가 이 값 이하일 때 공격을 시작할 거리를 반환합니다. */
	float GetAttackRange() const { return AttackRange; }


// ─────────────────────────────────────────────────────────────
// Attack Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 플레이어와의 수평 거리가 이 값 이하일 때 공격을 시작할 거리를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격", meta = (ClampMin = "0.0", UIMin = "0.0", Units = "cm"))
	float AttackRange = 750.0f;
};
