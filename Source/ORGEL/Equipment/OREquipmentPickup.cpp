#include "OREquipmentPickup.h"

#include "OREquipmentComponent.h"
#include "OREquipmentData.h"
#include "OREquipmentInstance.h"
#include "Character/Player/OGPlayerState.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"

void AOREquipmentPickup::SetEquipmentInstance(UOREquipmentInstance* InEquipmentInstance)
{
	// 장비 상태를 유지하도록 인스턴스를 보관하고 원본 데이터의 외형을 표시합니다.
	EquipmentInstance = InEquipmentInstance;
	SkeletalMeshComponent->SetSkeletalMeshAsset(InEquipmentInstance->GetEquipmentData()->GetMesh());
}

void AOREquipmentPickup::PerformInteraction(AActor* Interactor)
{
	// 상호작용한 플레이어에게 인스턴스를 그대로 장착하여 바닥에 있던 동안의 장비 상태를 유지합니다.
	CastChecked<APawn>(Interactor)->GetPlayerStateChecked<AOGPlayerState>()->GetEquipmentComponent()->Equip(EquipmentInstance);

	// 장비를 넘겨준 픽업은 제거합니다.
	Destroy();
}
