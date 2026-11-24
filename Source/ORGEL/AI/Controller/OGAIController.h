#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "OGAIController.generated.h"

class UBehaviorTree;

UCLASS()
class ORGEL_API AOGAIController : public AAIController
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Lifecycle
// ─────────────────────────────────────────────────────────────
public:
	/** 플레이어 접근 여부를 주기적으로 확인하도록 설정합니다. */
	AOGAIController();


// ─────────────────────────────────────────────────────────────
// AI
// ─────────────────────────────────────────────────────────────
public:
	/** 플레이어를 추적 대상과 조준 대상으로 지정하고 행동 트리를 실행합니다. */
	void StartAI();


// ─────────────────────────────────────────────────────────────
// AI Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 빙의한 몬스터가 실행할 행동 트리를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
