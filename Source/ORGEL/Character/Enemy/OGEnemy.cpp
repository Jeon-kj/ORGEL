#include "OGEnemy.h"

#include "AbilitySystem/ORAbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ORHealthSet.h"
#include "AbilitySystem/Attribute/ORMovementSet.h"
#include "AI/Controller/OGAIController.h"
#include "Data/ORCharacterStatData.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Define/Define.h"

AOGEnemy::AOGEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 몬스터 자신이 소유할 ASC와 체력 속성 세트를 생성합니다.
	{
		ASC = CreateDefaultSubobject<UORAbilitySystemComponent>(TEXT("ASC"));
		HealthSet = CreateDefaultSubobject<UORHealthSet>(TEXT("HealthSet"));
		MovementSet = CreateDefaultSubobject<UORMovementSet>(TEXT("MovementSet"));
	}

	// 몬스터 캡슐을 전용 충돌 프로필로 설정하여 플레이어 탄환이 몬스터를 구분하도록 합니다.
	GetCapsuleComponent()->SetCollisionProfileName(ORCollisionProfiles::Enemy);

	// 전투방에서 생성된 몬스터도 AI 컨트롤러가 빙의하도록 설정합니다.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 이동 방향 대신 AI 컨트롤러의 조준 방향을 향해 회전 속도에 맞춰 회전하도록 설정합니다.
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void AOGEnemy::BeginPlay()
{
	// Blueprint 시작 처리 전에 ASC와 공통 능력치를 준비합니다.
	ASC->InitAbilityActorInfo(this, this);

	// 생성자에서 생성되어 자동 등록된 속성 세트를 초기화합니다.
	ASC->InitializeAttributeSets(*InitialStatData);

	// 부모의 공통 사망 이벤트를 등록합니다.
	BindAttributeEvents();

	Super::BeginPlay();

	// 빙의는 BeginPlay 이전에 끝나므로, ASC 초기화를 마친 이 시점에 행동 트리를 시작합니다.
	CastChecked<AOGAIController>(GetController())->StartAI();
}