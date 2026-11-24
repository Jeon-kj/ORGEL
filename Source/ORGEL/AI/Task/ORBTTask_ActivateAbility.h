#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ORBTTask_ActivateAbility.generated.h"

class UAbilitySystemComponent;
struct FAbilityEndedData;

UCLASS()
class ORGEL_API UORBTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Lifecycle
// ─────────────────────────────────────────────────────────────
public:
	/** 실행 중 상태를 보관하도록 노드 인스턴스를 사용하고 기본 발동 태그를 지정합니다. */
	UORBTTask_ActivateAbility();


// ─────────────────────────────────────────────────────────────
// Task
// ─────────────────────────────────────────────────────────────
protected:
	/** 태그에 해당하는 어빌리티를 발동하고 종료될 때까지 대기합니다. */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** 행동 트리가 태스크를 중단하면 진행 중인 어빌리티를 취소합니다. */
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** 태스크가 끝나면 어빌리티 종료 이벤트 연결을 해제합니다. */
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	/** 발동한 태그의 어빌리티가 끝나면 태스크를 성공으로 완료합니다. */
	void HandleAbilityEnded(const FAbilityEndedData& EndedData);


// ─────────────────────────────────────────────────────────────
// Task Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 발동할 어빌리티의 태그를 지정합니다. */
	UPROPERTY(EditAnywhere, Category = "변수|어빌리티")
	FGameplayTag AbilityTag;

	/** 태스크를 완료할 행동 트리 컴포넌트를 참조합니다. */
	TWeakObjectPtr<UBehaviorTreeComponent> OwnerComponent;

	/** 어빌리티를 발동한 ASC를 참조합니다. */
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** 어빌리티 종료 이벤트 연결을 해제할 핸들을 보관합니다. */
	FDelegateHandle AbilityEndedHandle;
};
