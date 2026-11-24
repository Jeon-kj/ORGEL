#include "OGAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/GameplayStatics.h"

AOGAIController::AOGAIController()
{

}

void AOGAIController::StartAI()
{
	// 행동 트리의 첫 실행부터 추적 대상을 사용하도록 블랙보드에 플레이어를 먼저 기록한 뒤 트리를 시작합니다.
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	UBlackboardComponent* BlackboardComponent = nullptr;
	UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);
	BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), Player);
	RunBehaviorTree(BehaviorTree);

	// 이동과 공격 중 항상 플레이어를 바라보도록 조준 대상으로 지정합니다.
	SetFocus(Player);
}
