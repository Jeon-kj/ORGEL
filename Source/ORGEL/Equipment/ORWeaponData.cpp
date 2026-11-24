#include "ORWeaponData.h"

#include "ORWeaponInstance.h"
#include "Combat/ORProjectile.h"

UORWeaponData::UORWeaponData()
{
	// 무기 데이터는 무기 슬롯에만 장착하도록 설정합니다.
	Slot = EOREquipmentSlot::Weapon;

	// 무기별 탄환 Blueprint를 지정하기 전에도 발사할 수 있도록 기본 탄환 클래스를 사용합니다.
	ProjectileClass = AORProjectile::StaticClass();
}

TSubclassOf<UOREquipmentInstance> UORWeaponData::GetInstanceClass() const
{
	// 장착 시 메시를 부착하는 무기 인스턴스를 사용합니다.
	return UORWeaponInstance::StaticClass();
}
