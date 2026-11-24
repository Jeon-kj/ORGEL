#include "OREquipmentPickup.h"

#include "OREquipmentData.h"
#include "OREquipmentInstance.h"
#include "Components/SkeletalMeshComponent.h"

void AOREquipmentPickup::SetEquipmentInstance(UOREquipmentInstance* InEquipmentInstance)
{
	// 장비 상태를 유지하도록 인스턴스를 보관하고 원본 데이터의 외형을 표시합니다.
	EquipmentInstance = InEquipmentInstance;
	SkeletalMeshComponent->SetSkeletalMeshAsset(InEquipmentInstance->GetEquipmentData()->GetMesh());
}
