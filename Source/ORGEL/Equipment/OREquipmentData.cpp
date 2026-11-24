#include "OREquipmentData.h"

#include "OREquipmentInstance.h"

TSubclassOf<UOREquipmentInstance> UOREquipmentData::GetInstanceClass() const
{
	// 장착 시 외형이 없는 방어구·신발·이펙터는 공통 인스턴스를 사용합니다.
	return UOREquipmentInstance::StaticClass();
}
