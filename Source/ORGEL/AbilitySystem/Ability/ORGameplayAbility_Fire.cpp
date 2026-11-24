#include "ORGameplayAbility_Fire.h"

#include "GameplayEffect.h"
#include "ORGameplayTags.h"
#include "AbilitySystem/ORAbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ORCombatSet.h"
#include "Character/OGCharacter.h"
#include "Combat/ORProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Equipment/ORWeaponData.h"
#include "Equipment/ORWeaponInstance.h"

UORGameplayAbility_Fire::UORGameplayAbility_Fire()
{
	// 클릭한 순간에만 한 발을 발사하며 이동과 다른 어빌리티를 취소하지 않도록 설정합니다.
	{
		ActivationPolicy = EORAbilityActivationPolicy::OnInputTriggered;
		ActivationGroup = EORAbilityActivationGroup::Independent;
		InputPriority = 0;
		bCancelOnInputReleased = false;
	}

	// 구르기와 사망 중에는 발사하지 않도록 차단합니다.
	{
		ActivationBlockedTags.AddTag(ORGameplayTags::Status_Dodging);
		ActivationBlockedTags.AddTag(ORGameplayTags::Status_Dead);
	}

	// 연사 간격 판정에 사용할 쿨타임 태그를 등록합니다.
	CooldownTags.AddTag(ORGameplayTags::Cooldown_Attack);
}

void UORGameplayAbility_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 비용과 쿨타임을 확정하며, 확정에 실패하면 발사하지 않고 종료합니다.
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	// 무기 피해량에 공격력 배율을 곱한 피해 효과를 이번 발사의 모든 탄환이 공유합니다.
	UORWeaponInstance* WeaponInstance = GetWeaponInstance();
	const UORWeaponData* WeaponData = CastChecked<UORWeaponData>(WeaponInstance->GetEquipmentData());
	FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	DamageSpec.Data->SetSetByCallerMagnitude(ORGameplayTags::SetByCaller_Damage, WeaponData->GetDamage() * GetORAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UORCombatSet::GetAttackPowerAttribute()));

	// 총구 위치에서 캐릭터의 수평 조준 방향을 기준으로 발사합니다.
	AOGCharacter* Character = GetOGCharacterFromActorInfo();
	const FVector MuzzleLocation = WeaponInstance->GetMeshComponent()->GetSocketLocation(WeaponData->GetMuzzleSocketName());
	const float AimYaw = Character->GetActorRotation().Yaw;

	// 여러 발은 확산 각도 안에 양 끝을 포함해 균등하게 나누고, 한 발은 정면으로 발사합니다.
	const int32 ProjectileCount = WeaponData->GetProjectileCount();
	const float SpreadAngle = WeaponData->GetSpreadAngle();
	for (int32 Index = 0; Index < ProjectileCount; ++Index)
	{
		const float YawOffset = ProjectileCount > 1 ? -SpreadAngle * 0.5f + SpreadAngle * Index / (ProjectileCount - 1) : 0.0f;
		const FTransform SpawnTransform(FRotator(0.0f, AimYaw + YawOffset, 0.0f), MuzzleLocation);

		// 생성 직후 겹침에도 피해가 적용되도록 수치를 설정한 뒤 생성을 완료합니다.
		AORProjectile* Projectile = GetWorld()->SpawnActorDeferred<AORProjectile>(WeaponData->GetProjectileClass(), SpawnTransform, Character, Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->InitializeProjectile(*WeaponData, DamageSpec);
		Projectile->FinishSpawning(SpawnTransform);
	}

	// 사격 애니메이션은 발사와 독립적으로 재생합니다.
	if (FireMontage)
	{
		Character->PlayAnimMontage(FireMontage);
	}

	// 발사를 마친 즉시 어빌리티를 종료합니다.
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

const FGameplayTagContainer* UORGameplayAbility_Fire::GetCooldownTags() const
{
	return &CooldownTags;
}

void UORGameplayAbility_Fire::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	// 무기 공격속도에 공격속도 배율을 곱한 초당 발사 횟수로 다음 발사까지의 간격을 계산합니다.
	const UORWeaponData* WeaponData = CastChecked<UORWeaponData>(GetWeaponInstance()->GetEquipmentData());
	UORAbilitySystemComponent* ASC = GetORAbilitySystemComponentFromActorInfo();
	const float FireInterval = 1.0f / (WeaponData->GetFireRate() * ASC->GetNumericAttribute(UORCombatSet::GetAttackSpeedAttribute()));

	// 간격 동안 쿨타임 태그를 부여하여 그 사이의 클릭을 발동 검사에서 거부합니다.
	ASC->AddTimedTag(ORGameplayTags::Cooldown_Attack, FireInterval);
}

UORWeaponInstance* UORGameplayAbility_Fire::GetWeaponInstance() const
{
	return CastChecked<UORWeaponInstance>(GetCurrentSourceObject());
}
