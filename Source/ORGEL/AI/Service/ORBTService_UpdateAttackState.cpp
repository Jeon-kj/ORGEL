#include "ORBTService_UpdateAttackState.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "ORGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Character/Enemy/Normal/OGNormalEnemy.h"

UORBTService_UpdateAttackState::UORBTService_UpdateAttackState()
{
	// 공격 가능 여부를 0.1초 간격으로 갱신하며, 분기 진입 시에도 즉시 갱신합니다.
	{
		NodeName = TEXT("Update Attack State");
		Interval = 0.1f;
		RandomDeviation = 0.0f;
		bCallTickOnSearchStart = true;
	}

	// 기본 블랙보드 키 이름을 지정하고 키 종류를 대상 액터와 불리언으로 제한합니다.
	{
		TargetActorKey.SelectedKeyName = TEXT("TargetActor");
		TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetActorKey), AActor::StaticClass());
		CanAttackKey.SelectedKeyName = TEXT("CanAttack");
		CanAttackKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, CanAttackKey));
	}
}

void UORBTService_UpdateAttackState::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// 행동 트리에 연결된 블랙보드에서 키 이름에 해당하는 키를 찾아 연결합니다.
	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	TargetActorKey.ResolveSelectedKey(*BlackboardAsset);
	CanAttackKey.ResolveSelectedKey(*BlackboardAsset);
}

void UORBTService_UpdateAttackState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 몬스터와 대상 사이의 수평 거리가 공격 거리 이하인지 확인합니다.
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	const AOGNormalEnemy* Enemy = CastChecked<AOGNormalEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	const AActor* Target = CastChecked<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	const bool bInRange = FVector::DistSquared2D(Enemy->GetActorLocation(), Target->GetActorLocation()) <= FMath::Square(Enemy->GetAttackRange());

	// 거리 안에 있고 다음 공격 대기가 끝난 경우에만 공격 가능으로 기록합니다.
	Blackboard->SetValueAsBool(CanAttackKey.SelectedKeyName, bInRange && !Enemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(ORGameplayTags::Cooldown_Attack));
}
