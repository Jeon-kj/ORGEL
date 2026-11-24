#pragma once

#include "CoreMinimal.h"
#include "OREquipmentData.h"
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
// Attachment
// ─────────────────────────────────────────────────────────────
public:
	/** 무기 메시를 부착할 캐릭터 메시의 소켓 이름을 반환합니다. */
	FName GetAttachSocketName() const { return AttachSocketName; }


// ─────────────────────────────────────────────────────────────
// Attachment Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 무기 메시를 부착할 캐릭터 메시의 소켓 이름을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|부착")
	FName AttachSocketName = TEXT("WeaponSocket");
};
