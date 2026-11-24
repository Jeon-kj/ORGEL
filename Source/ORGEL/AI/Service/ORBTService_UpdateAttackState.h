#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ORBTService_UpdateAttackState.generated.h"

UCLASS()
class ORGEL_API UORBTService_UpdateAttackState : public UBTService
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Lifecycle
// ─────────────────────────────────────────────────────────────
public:
	/** 0.1초마다 공격 가능 여부를 갱신하고 사용할 블랙보드 키를 설정합니다. */
	UORBTService_UpdateAttackState();

protected:
	/** 행동 트리의 블랙보드에서 지정한 키를 찾아 연결합니다. */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;


// ─────────────────────────────────────────────────────────────
// Service
// ─────────────────────────────────────────────────────────────
protected:
	/** 대상이 공격 거리 안에 있고 다음 공격 대기가 끝났는지 블랙보드에 기록합니다. */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


// ─────────────────────────────────────────────────────────────
// Blackboard Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 거리를 계산할 추적 대상 키를 지정합니다. */
	UPROPERTY(EditAnywhere, Category = "변수|블랙보드")
	FBlackboardKeySelector TargetActorKey;

	/** 공격 가능 여부를 기록할 키를 지정합니다. */
	UPROPERTY(EditAnywhere, Category = "변수|블랙보드")
	FBlackboardKeySelector CanAttackKey;
};
