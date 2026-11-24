#pragma once

#include "CoreMinimal.h"
#include "Interact/ORInteractableObject.h"
#include "OREquipmentPickup.generated.h"

class UOREquipmentInstance;

UCLASS()
class ORGEL_API AOREquipmentPickup : public AORInteractableObject
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Equipment
// ─────────────────────────────────────────────────────────────
public:
	/** 바닥에 놓을 장비 인스턴스를 보관하고 장비의 외형 메시를 표시합니다. */
	void SetEquipmentInstance(UOREquipmentInstance* InEquipmentInstance);


// ─────────────────────────────────────────────────────────────
// Equipment Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 해제되어 바닥에 놓인 장비의 상태가 유지되도록 인스턴스를 보관합니다. */
	UPROPERTY(Transient)
	TObjectPtr<UOREquipmentInstance> EquipmentInstance;
};
