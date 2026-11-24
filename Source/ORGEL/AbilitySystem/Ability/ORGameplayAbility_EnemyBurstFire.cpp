#include "ORGameplayAbility_EnemyBurstFire.h"

#include "GameplayEffect.h"
#include "ORGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/ORAbilitySystemComponent.h"
#include "Character/OGCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UORGameplayAbility_EnemyBurstFire::UORGameplayAbility_EnemyBurstFire()
{
	// AI 행동 트리가 태그로 발동할 수 있도록 공격 태그를 지정합니다.
	SetAssetTags(FGameplayTagContainer(ORGameplayTags::Ability_Enemy_Attack));

	// 사망 중에는 발동하지 않도록 차단합니다.
	ActivationBlockedTags.AddTag(ORGameplayTags::Status_Dead);

	// 다음 공격 대기 판정에 사용할 쿨타임 태그를 등록합니다.
	CooldownTags.AddTag(ORGameplayTags::Cooldown_Attack);
}

void UORGameplayAbility_EnemyBurstFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 새 공격마다 발사 수를 초기화하고 준비 시간이 지난 뒤 첫 발을 쏩니다.
	FiredShotCount = 0;
	WaitForNextShot(PrepareTime);
}

const FGameplayTagContainer* UORGameplayAbility_EnemyBurstFire::GetCooldownTags() const
{
	return &CooldownTags;
}

void UORGameplayAbility_EnemyBurstFire::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	// 대기 시간 동안 쿨타임 태그를 부여하여 그 사이의 공격 발동을 거부합니다.
	GetORAbilitySystemComponentFromActorInfo()->AddTimedTag(ORGameplayTags::Cooldown_Attack, CooldownDuration);
}

void UORGameplayAbility_EnemyBurstFire::FireShot()
{
	// 나팔 끝에서 발사 순간의 플레이어 수평 방향으로 탄환을 발사합니다.
	AOGCharacter* Character = GetOGCharacterFromActorInfo();
	const FVector MuzzleLocation = Character->GetMesh()->GetSocketLocation(MuzzleSocketName);
	const FVector TargetLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
	const FTransform SpawnTransform((TargetLocation - MuzzleLocation).GetSafeNormal2D().Rotation(), MuzzleLocation);

	// 몬스터 피해량을 담은 피해 효과로 탄환을 초기화한 뒤 생성을 완료합니다.
	FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	DamageSpec.Data->SetSetByCallerMagnitude(ORGameplayTags::SetByCaller_Damage, Damage);
	AORProjectile* Projectile = GetWorld()->SpawnActorDeferred<AORProjectile>(ProjectileClass, SpawnTransform, Character, Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Projectile->InitializeProjectile(ProjectileParams, DamageSpec);
	Projectile->FinishSpawning(SpawnTransform);

	// 반동 연출만 재생하며, 피격 반동과 구분되도록 탄환 생성은 애니메이션과 무관하게 시간으로만 처리합니다.
	Character->PlayAnimMontage(AttackMontage);

	// 남은 발이 있으면 다음 발을 예약합니다.
	if (++FiredShotCount < ShotCount)
	{
		WaitForNextShot(ShotInterval);
		return;
	}

	// 마지막 발을 쏜 시점부터 다음 공격 대기를 시작하고 공격을 종료합니다.
	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UORGameplayAbility_EnemyBurstFire::WaitForNextShot(float Delay)
{
	// 어빌리티가 취소되면 함께 종료되는 대기 태스크로 다음 발사를 예약합니다.
	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, Delay);
	WaitTask->OnFinish.AddDynamic(this, &ThisClass::FireShot);
	WaitTask->ReadyForActivation();
}
