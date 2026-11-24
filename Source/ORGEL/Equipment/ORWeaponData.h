#pragma once

#include "CoreMinimal.h"
#include "OREquipmentData.h"
#include "ORWeaponData.generated.h"

class AORProjectile;

UCLASS()
class ORGEL_API UORWeaponData : public UOREquipmentData
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Initialization
// ─────────────────────────────────────────────────────────────
public:
	/** 무기 데이터가 항상 무기 슬롯을 사용하도록 설정합니다. */
	UORWeaponData();


// ─────────────────────────────────────────────────────────────
// Instance
// ─────────────────────────────────────────────────────────────
public:
	/** 장착 시 외형 메시를 무기 소켓에 부착하는 무기 인스턴스 클래스를 반환합니다. */
	virtual TSubclassOf<UOREquipmentInstance> GetInstanceClass() const override;


// ─────────────────────────────────────────────────────────────
// Weapon Stats
// ─────────────────────────────────────────────────────────────
public:
	/** 탄환 한 발이 적에게 주는 기본 피해량을 반환합니다. */
	float GetDamage() const { return Damage; }

	/** 초당 발사 횟수를 반환합니다. */
	float GetFireRate() const { return FireRate; }

	/** 탄환 사거리를 반환하며, 0이면 사거리 제한이 없습니다. */
	float GetRange() const { return Range; }

	/** 탄환의 이동 속도를 반환합니다. */
	float GetProjectileSpeed() const { return ProjectileSpeed; }

	/** 탄환의 충돌 반지름을 반환합니다. */
	float GetProjectileRadius() const { return ProjectileRadius; }

	/** 1회 발사 시 생성할 탄환 수를 반환합니다. */
	int32 GetProjectileCount() const { return ProjectileCount; }

	/** 여러 탄환을 균등하게 나눌 전방 부채꼴 각도를 반환합니다. */
	float GetSpreadAngle() const { return SpreadAngle; }

	/** 탄환이 벽에서 반사될 수 있는 최대 횟수를 반환합니다. */
	int32 GetMaxBounceCount() const { return MaxBounceCount; }

	/** 발사할 탄환 액터 클래스를 반환합니다. */
	TSubclassOf<AORProjectile> GetProjectileClass() const { return ProjectileClass; }


// ─────────────────────────────────────────────────────────────
// Weapon Stat Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 탄환 한 발이 적에게 주는 기본 피해량을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Damage = 10.0f;

	/** 초당 발사 횟수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기", meta = (ClampMin = "0.01", UIMin = "0.01"))
	float FireRate = 2.0f;

	/** 탄환 사거리를 지정하며, 0이면 사거리 제한이 없습니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기", meta = (ClampMin = "0.0", UIMin = "0.0", Units = "cm"))
	float Range = 900.0f;

	/** 탄환의 이동 속도를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기", meta = (ClampMin = "1.0", UIMin = "1.0", Units = "cm/s"))
	float ProjectileSpeed = 1500.0f;

	/** 탄환의 충돌 반지름을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기", meta = (ClampMin = "0.1", UIMin = "0.1", Units = "cm"))
	float ProjectileRadius = 10.0f;

	/** 1회 발사 시 생성할 탄환 수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기", meta = (ClampMin = "1", UIMin = "1"))
	int32 ProjectileCount = 1;

	/** 여러 탄환을 균등하게 나눌 전방 부채꼴 각도를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기", meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0", Units = "deg"))
	float SpreadAngle = 0.0f;

	/** 탄환이 벽에서 반사될 수 있는 최대 횟수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기", meta = (ClampMin = "0", UIMin = "0"))
	int32 MaxBounceCount = 0;

	/** 발사할 탄환 액터 클래스를 지정하며, 무기별 외형은 하위 Blueprint에서 설정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기")
	TSubclassOf<AORProjectile> ProjectileClass;


// ─────────────────────────────────────────────────────────────
// Attachment
// ─────────────────────────────────────────────────────────────
public:
	/** 무기 메시를 부착할 캐릭터 메시의 소켓 이름을 반환합니다. */
	FName GetAttachSocketName() const { return AttachSocketName; }

	/** 탄환을 생성할 무기 메시의 총구 소켓 이름을 반환합니다. */
	FName GetMuzzleSocketName() const { return MuzzleSocketName; }


// ─────────────────────────────────────────────────────────────
// Attachment Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 무기 메시를 부착할 캐릭터 메시의 소켓 이름을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|부착")
	FName AttachSocketName = TEXT("WeaponSocket");

	/** 탄환을 생성할 무기 메시의 총구 소켓 이름을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|부착")
	FName MuzzleSocketName = TEXT("MuzzleSocket");
};
