#pragma once

#include "CoreMinimal.h"
#include "OREquipmentInstance.h"
#include "ORWeaponInstance.generated.h"

class USkeletalMeshComponent;

UCLASS()
class ORGEL_API UORWeaponInstance : public UOREquipmentInstance
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Pawn
// ─────────────────────────────────────────────────────────────
public:
	/** 기존 Pawn의 무기 메시를 제거하고 새 Pawn의 무기 소켓에 무기 메시를 부착합니다. */
	virtual void SetPawn(APawn* Pawn) override;


// ─────────────────────────────────────────────────────────────
// Weapon Mesh
// ─────────────────────────────────────────────────────────────
public:
	/** 현재 Pawn에 부착한 무기 메시를 반환하며, Pawn이 없으면 nullptr를 반환합니다. */
	USkeletalMeshComponent* GetMeshComponent() const { return MeshComponent; }


// ─────────────────────────────────────────────────────────────
// Weapon Mesh Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 현재 Pawn에 부착한 무기 메시를 참조하며, Pawn과 함께 먼저 제거될 수 있습니다. */
	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
};
