#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "ORContactDamageComponent.generated.h"

class UGameplayEffect;

UCLASS()
class ORGEL_API UORContactDamageComponent : public UCapsuleComponent
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Lifecycle
// ─────────────────────────────────────────────────────────────
public:
	/** 플레이어와만 겹치는 접촉 판정 캡슐을 설정하고 겹침 이벤트를 연결합니다. */
	UORContactDamageComponent();

protected:
	/** 소유 캐릭터 캡슐보다 여유값만큼만 큰 크기로 판정 범위를 맞춥니다. */
	virtual void BeginPlay() override;


// ─────────────────────────────────────────────────────────────
// Contact
// ─────────────────────────────────────────────────────────────
private:
	/** 플레이어와 닿으면 즉시 피해를 시도하고 반복 시도를 시작합니다. */
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 플레이어와 떨어지면 반복 시도를 중지합니다. */
	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** 무적이 아닌 접촉 대상에게 접촉 피해 효과를 적용합니다. */
	void ApplyContactDamage();


// ─────────────────────────────────────────────────────────────
// Contact Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 접촉한 플레이어에게 주는 피해량을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|접촉피해", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Damage = 0.5f;

	/** 닿아 있는 동안 피해를 다시 시도할 간격을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|접촉피해", meta = (ClampMin = "0.01", UIMin = "0.01", Units = "s"))
	float DamageInterval = 0.1f;

	/** 서로 막는 캡슐이 맞닿았을 때 겹침을 감지할 수 있도록 소유 캐릭터 캡슐보다 넓힐 최소 여유값을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|접촉피해", meta = (ClampMin = "0.0", UIMin = "0.0", Units = "cm"))
	float Padding = 2.0f;

	/** 접촉한 플레이어에게 적용할 SetByCaller 피해 효과를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|접촉피해")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** 닿아 있는 동안 피해를 반복 시도할 타이머를 보관합니다. */
	FTimerHandle DamageTimerHandle;

	/** 현재 닿아 있는 피해 대상을 참조합니다. */
	TWeakObjectPtr<AActor> ContactTarget;
};
