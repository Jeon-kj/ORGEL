#pragma once

#include "CoreMinimal.h"
#include "ORGameplayAbility.h"
#include "ORGameplayAbility_Fire.generated.h"

class UGameplayEffect;
class UORWeaponInstance;

UCLASS(Abstract)
class ORGEL_API UORGameplayAbility_Fire : public UORGameplayAbility
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Gameplay Ability
// ─────────────────────────────────────────────────────────────
public:
	/** 클릭마다 한 번 발동하고 구르기와 사망 중에는 발동하지 않도록 설정합니다. */
	UORGameplayAbility_Fire();

	/** 장착 무기의 수치로 탄환을 발사하고 즉시 종료합니다. */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** 연사 간격 판정에 사용할 공격 쿨타임 태그를 반환합니다. */
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	/** 무기 공격속도와 공격속도 배율로 계산한 연사 간격만큼 공격 쿨타임 태그를 부여합니다. */
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;


// ─────────────────────────────────────────────────────────────
// Weapon
// ─────────────────────────────────────────────────────────────
private:
	/** 이 어빌리티를 부여한 무기 인스턴스를 반환합니다. */
	UORWeaponInstance* GetWeaponInstance() const;


// ─────────────────────────────────────────────────────────────
// Fire Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 탄환이 명중한 몬스터에게 적용할 SetByCaller 피해 효과를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** 발사할 때 재생할 사격 몽타주를 지정하며, 비워 두면 재생하지 않습니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	TObjectPtr<UAnimMontage> FireMontage;

	/** 공격 쿨타임 판정에 사용할 태그 묶음을 보관합니다. */
	FGameplayTagContainer CooldownTags;
};
