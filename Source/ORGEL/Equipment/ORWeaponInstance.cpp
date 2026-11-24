#include "ORWeaponInstance.h"

#include "ORWeaponData.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

void UORWeaponInstance::SetPawn(APawn* Pawn)
{
	// 이전 Pawn에 부착한 메시를 제거하며, Pawn과 함께 이미 제거된 메시는 건너뜁니다.
	if (IsValid(MeshComponent))
	{
		MeshComponent->DestroyComponent();
	}
	MeshComponent = nullptr;

	// 외형을 표시할 Pawn이 없으면 제거만 수행합니다.
	if (!Pawn)
	{
		return;
	}

	// 충돌 없이 외형만 표시하는 무기 메시를 생성하여 캐릭터 메시의 무기 소켓에 부착합니다.
	const UORWeaponData* WeaponData = CastChecked<UORWeaponData>(GetEquipmentData());
	MeshComponent = NewObject<USkeletalMeshComponent>(Pawn);
	MeshComponent->SetSkeletalMeshAsset(WeaponData->GetMesh());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(CastChecked<ACharacter>(Pawn)->GetMesh(), WeaponData->GetAttachSocketName());

	// 메시마다 다른 정면 축과 크기를 무기 데이터의 회전과 스케일로 소켓 기준에서 보정합니다.
	MeshComponent->SetRelativeRotation(WeaponData->GetAttachRotation());
	MeshComponent->SetRelativeScale3D(WeaponData->GetAttachScale());
	MeshComponent->RegisterComponent();
}
