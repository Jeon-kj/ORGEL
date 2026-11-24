#include "OREquipmentPickup.h"

#include "OREquipmentComponent.h"
#include "OREquipmentData.h"
#include "OREquipmentInstance.h"
#include "Character/Player/OGPlayerState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Define/Define.h"
#include "GameFramework/Pawn.h"

AOREquipmentPickup::AOREquipmentPickup()
{
	// 무기 메시에는 물리 에셋이 없어 충돌을 만들 수 없으므로, 외형 메시는 충돌 없이 표시만 합니다.
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 상호작용 탐색에만 반응하고 플레이어 이동과 탄환은 막지 않는 구체를 생성합니다.
	{
		InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
		InteractionSphere->SetupAttachment(SkeletalMeshComponent);
		InteractionSphere->SetSphereRadius(50.0f);
		InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InteractionSphere->SetCollisionObjectType(ECC_WorldDynamic);
		InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
		InteractionSphere->SetCollisionResponseToChannel(ORCollisionChannels::Interaction, ECR_Block);
		InteractionSphere->SetGenerateOverlapEvents(false);
	}
}

void AOREquipmentPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 배치한 장비 데이터가 있으면 레벨에 놓은 시점부터 외형을 표시합니다.
	if (InitialEquipmentData)
	{
		SkeletalMeshComponent->SetSkeletalMeshAsset(InitialEquipmentData->GetMesh());
	}
}

void AOREquipmentPickup::BeginPlay()
{
	Super::BeginPlay();

	// 레벨에 직접 배치한 픽업은 줍기 전까지 상태를 유지할 새 장비 인스턴스를 생성합니다.
	if (InitialEquipmentData)
	{
		UOREquipmentInstance* NewInstance = NewObject<UOREquipmentInstance>(GetWorld(), InitialEquipmentData->GetInstanceClass());
		NewInstance->SetEquipmentData(InitialEquipmentData);
		SetEquipmentInstance(NewInstance);
	}
}

void AOREquipmentPickup::SetEquipmentInstance(UOREquipmentInstance* InEquipmentInstance)
{
	// 장비 상태를 유지하도록 인스턴스를 보관하고 원본 데이터의 외형을 표시합니다.
	EquipmentInstance = InEquipmentInstance;
	SkeletalMeshComponent->SetSkeletalMeshAsset(InEquipmentInstance->GetEquipmentData()->GetMesh());
}

bool AOREquipmentPickup::CanInteract(const AActor* Interactor) const
{
	// 장비 데이터 없이 배치된 픽업은 장착할 장비가 없으므로 상호작용을 거부합니다.
	return Super::CanInteract(Interactor) && EquipmentInstance;
}

void AOREquipmentPickup::PerformInteraction(AActor* Interactor)
{
	// 상호작용한 플레이어에게 인스턴스를 그대로 장착하여 바닥에 있던 동안의 장비 상태를 유지합니다.
	CastChecked<APawn>(Interactor)->GetPlayerStateChecked<AOGPlayerState>()->GetEquipmentComponent()->Equip(EquipmentInstance);

	// 장비를 넘겨준 픽업은 제거합니다.
	Destroy();
}
