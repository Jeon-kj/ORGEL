#pragma once

#include "CoreMinimal.h"
#include "ORGameplayAbility.h"
#include "Combat/ORProjectile.h"
#include "ORGameplayAbility_EnemyBurstFire.generated.h"

class UGameplayEffect;

UCLASS()
class ORGEL_API UORGameplayAbility_EnemyBurstFire : public UORGameplayAbility
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Gameplay Ability
// ─────────────────────────────────────────────────────────────
public:
	/** AI가 발동할 공격 태그를 지정하고 사망 중에는 발동하지 않도록 설정합니다. */
	UORGameplayAbility_EnemyBurstFire();

	/** 준비 시간이 지난 뒤 첫 탄환을 발사하도록 대기를 시작합니다. */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** 다음 공격까지의 대기 판정에 사용할 쿨타임 태그를 반환합니다. */
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	/** 마지막 탄환 발사 후 대기 시간만큼 쿨타임 태그를 부여합니다. */
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;


// ─────────────────────────────────────────────────────────────
// Fire
// ─────────────────────────────────────────────────────────────
private:
	/** 발사 순간의 플레이어 방향으로 한 발을 쏘고, 남은 발이 있으면 다음 발을 예약합니다. */
	UFUNCTION()
	void FireShot();

	/** 지정한 시간이 지나면 다음 탄환을 발사하도록 대기를 시작합니다. */
	void WaitForNextShot(float Delay);


// ─────────────────────────────────────────────────────────────
// Fire Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 공격을 시작한 뒤 첫 탄환을 발사하기까지 정지한 채 조준할 시간을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격", meta = (ClampMin = "0.0", UIMin = "0.0", Units = "s"))
	float PrepareTime = 0.5f;

	/** 한 번의 공격에서 순차 발사할 탄환 수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격", meta = (ClampMin = "1", UIMin = "1"))
	int32 ShotCount = 3;

	/** 연속 발사하는 탄환 사이의 간격을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격", meta = (ClampMin = "0.0", UIMin = "0.0", Units = "s"))
	float ShotInterval = 0.3f;

	/** 마지막 탄환 발사 후 다음 공격을 시작할 수 있을 때까지의 시간을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격", meta = (ClampMin = "0.0", UIMin = "0.0", Units = "s"))
	float CooldownDuration = 2.0f;

	/** 탄환이 플레이어에게 주는 피해량을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Damage = 0.5f;

	/** 탄환을 생성할 캐릭터 메시의 소켓 또는 본 이름을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	FName MuzzleSocketName = TEXT("fx_muzzle");

	/** 발사할 탄환의 반지름, 탄속, 사거리, 도탄 횟수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	FORProjectileParams ProjectileParams;

	/** 발사할 탄환 액터 클래스를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	TSubclassOf<AORProjectile> ProjectileClass;

	/** 탄환이 명중한 플레이어에게 적용할 SetByCaller 피해 효과를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** 탄환을 발사할 때마다 재생할 반동 몽타주를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|공격")
	TObjectPtr<UAnimMontage> AttackMontage;

	/** 다음 공격 대기 판정에 사용할 쿨타임 태그 묶음을 보관합니다. */
	FGameplayTagContainer CooldownTags;

	/** 이번 공격에서 이미 발사한 탄환 수를 보관합니다. */
	int32 FiredShotCount = 0;
};
