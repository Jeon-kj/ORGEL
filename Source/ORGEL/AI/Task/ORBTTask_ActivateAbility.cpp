#include "ORBTTask_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "ORGameplayTags.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UORBTTask_ActivateAbility::UORBTTask_ActivateAbility()
{
	// 어빌리티 종료를 기다리는 동안 대상 컴포넌트와 이벤트 핸들을 보관하도록 노드 인스턴스를 사용합니다.
	{
		NodeName = TEXT("Activate Ability");
		bCreateNodeInstance = true;
	}

	// 몬스터 공격 어빌리티를 기본 발동 대상으로 지정합니다.
	AbilityTag = ORGameplayTags::Ability_Enemy_Attack;
}

EBTNodeResult::Type UORBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 쿨타임 등 발동 조건을 만족하지 못하면 실패로 처리합니다.
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner()->GetPawn());
	if (!ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag)))
	{
		return EBTNodeResult::Failed;
	}

	// 발동과 동시에 끝난 어빌리티는 기다리지 않고 바로 성공으로 처리합니다.
	TArray<FGameplayAbilitySpec*> MatchingSpecs;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(AbilityTag), MatchingSpecs, false);
	if (!MatchingSpecs.ContainsByPredicate([](const FGameplayAbilitySpec* Spec) { return Spec->IsActive(); }))
	{
		return EBTNodeResult::Succeeded;
	}

	// 진행 중인 어빌리티가 끝날 때 태스크를 완료하도록 종료 이벤트를 연결합니다.
	OwnerComponent = &OwnerComp;
	AbilitySystemComponent = ASC;
	AbilityEndedHandle = ASC->OnAbilityEnded.AddUObject(this, &ThisClass::HandleAbilityEnded);
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UORBTTask_ActivateAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 취소로 발생하는 종료 이벤트가 태스크를 다시 완료하지 않도록 연결을 먼저 해제한 뒤 어빌리티를 취소합니다.
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		ASC->OnAbilityEnded.Remove(AbilityEndedHandle);
		const FGameplayTagContainer AbilityTags(AbilityTag);
		ASC->CancelAbilities(&AbilityTags);
	}

	return EBTNodeResult::Aborted;
}

void UORBTTask_ActivateAbility::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// 완료 방식과 관계없이 남아 있는 종료 이벤트 연결을 해제합니다.
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		ASC->OnAbilityEnded.Remove(AbilityEndedHandle);
	}
	AbilitySystemComponent.Reset();

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UORBTTask_ActivateAbility::HandleAbilityEnded(const FAbilityEndedData& EndedData)
{
	// 발동한 태그의 어빌리티가 끝난 경우에만 태스크를 성공으로 완료합니다.
	if (EndedData.AbilityThatEnded->GetAssetTags().HasTagExact(AbilityTag))
	{
		FinishLatentTask(*OwnerComponent.Get(), EBTNodeResult::Succeeded);
	}
}
