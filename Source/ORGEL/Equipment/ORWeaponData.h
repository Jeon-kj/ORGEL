#pragma once

#include "CoreMinimal.h"
#include "OREquipmentData.h"
#include "Combat/ORProjectile.h"
#include "ORWeaponData.generated.h"

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

	/** 발사할 탄환의 반지름, 탄속, 사거리, 도탄 횟수를 반환합니다. */
	const FORProjectileParams& GetProjectileParams() const { return ProjectileParams; }

	/** 1회 발사 시 생성할 탄환 수를 반환합니다. */
	int32 GetProjectileCount() const { return ProjectileCount; }

	/** 여러 탄환을 균등하게 나눌 전방 부채꼴 각도를 반환합니다. */
	float GetSpreadAngle() const { return SpreadAngle; }

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

	/** 발사할 탄환의 반지름, 탄속, 사거리, 도탄 횟수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기")
	FORProjectileParams ProjectileParams;

	/** 1회 발사 시 생성할 탄환 수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기", meta = (ClampMin = "1", UIMin = "1"))
	int32 ProjectileCount = 1;

	/** 여러 탄환을 균등하게 나눌 전방 부채꼴 각도를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기", meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0", Units = "deg"))
	float SpreadAngle = 0.0f;

	/** 발사할 탄환 액터 클래스를 지정하며, 무기별 외형은 하위 Blueprint에서 설정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|무기")
	TSubclassOf<AORProjectile> ProjectileClass;


// ─────────────────────────────────────────────────────────────
// Attachment
// ─────────────────────────────────────────────────────────────
public:
	/** 무기 메시를 부착할 캐릭터 메시의 소켓 이름을 반환합니다. */
	FName GetAttachSocketName() const { return AttachSocketName; }

	/** 무기 소켓에 부착한 무기 메시에 적용할 상대 회전을 반환합니다. */
	FRotator GetAttachRotation() const { return AttachRotation; }

	/** 무기 소켓에 부착한 무기 메시에 적용할 상대 스케일을 반환합니다. */
	FVector GetAttachScale() const { return AttachScale; }

	/** 탄환을 생성할 무기 메시의 총구 소켓 이름을 반환합니다. */
	FName GetMuzzleSocketName() const { return MuzzleSocketName; }


// ─────────────────────────────────────────────────────────────
// Attachment Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 무기 메시를 부착할 캐릭터 메시의 소켓 이름을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|부착")
	FName AttachSocketName = TEXT("WeaponSocket");

	/** 메시마다 다른 정면 축을 맞추도록 무기 소켓 기준으로 적용할 무기 메시의 회전을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|부착")
	FRotator AttachRotation = FRotator::ZeroRotator;

	/** 무기 소켓 기준으로 적용할 무기 메시의 크기 배율을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|부착")
	FVector AttachScale = FVector::OneVector;

	/** 탄환을 생성할 무기 메시의 총구 소켓 이름을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|부착")
	FName MuzzleSocketName = TEXT("MuzzleSocket");
};
