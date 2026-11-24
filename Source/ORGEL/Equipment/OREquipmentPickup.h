#pragma once

#include "CoreMinimal.h"
#include "Interact/ORInteractableObject.h"
#include "OREquipmentPickup.generated.h"

class UOREquipmentData;
class UOREquipmentInstance;
class USphereComponent;

UCLASS()
class ORGEL_API AOREquipmentPickup : public AORInteractableObject
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Lifecycle
// ─────────────────────────────────────────────────────────────
public:
	/** 상호작용 탐색용 구체를 생성하고, 외형 메시는 충돌 없이 표시만 하도록 설정합니다. */
	AOREquipmentPickup();

	/** 레벨에 배치한 장비 데이터의 외형을 에디터와 게임에서 표시합니다. */
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	/** 레벨에 배치한 장비 데이터가 있으면 새 장비 인스턴스를 생성하여 보관합니다. */
	virtual void BeginPlay() override;


// ─────────────────────────────────────────────────────────────
// Equipment
// ─────────────────────────────────────────────────────────────
public:
	/** 바닥에 놓을 장비 인스턴스를 보관하고 장비의 외형 메시를 표시합니다. */
	void SetEquipmentInstance(UOREquipmentInstance* InEquipmentInstance);


// ─────────────────────────────────────────────────────────────
// Interaction
// ─────────────────────────────────────────────────────────────
protected:
	/** 공통 조건과 함께 보관한 장비가 있을 때만 상호작용을 허용합니다. */
	virtual bool CanInteract(const AActor* Interactor) const override;

	/** 보관한 장비를 상호작용한 플레이어에게 장착하고 픽업을 제거합니다. */
	virtual void PerformInteraction(AActor* Interactor) override;


// ─────────────────────────────────────────────────────────────
// Components
// ─────────────────────────────────────────────────────────────
private:
	/** 외형 메시의 충돌과 관계없이 상호작용 탐색에만 반응하는 구체를 참조합니다. */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionSphere;


// ─────────────────────────────────────────────────────────────
// Equipment Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 레벨에 직접 배치할 때 놓을 장비 데이터를 지정하며, 해제로 떨어진 픽업은 비워 둡니다. */
	UPROPERTY(EditAnywhere, Category = "변수|장비")
	TObjectPtr<UOREquipmentData> InitialEquipmentData;

	/** 해제되어 바닥에 놓인 장비의 상태가 유지되도록 인스턴스를 보관합니다. */
	UPROPERTY(Transient)
	TObjectPtr<UOREquipmentInstance> EquipmentInstance;
};
