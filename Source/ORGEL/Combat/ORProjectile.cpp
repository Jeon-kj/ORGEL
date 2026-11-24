#include "ORProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SphereComponent.h"
#include "Define/Define.h"
#include "Equipment/ORWeaponData.h"
#include "GameFramework/ProjectileMovementComponent.h"

AORProjectile::AORProjectile()
{
	// 탄환 액터 자체에는 매 프레임 처리를 사용하지 않도록 설정합니다.
	PrimaryActorTick.bCanEverTick = false;

	// 생성 직후의 겹침도 처리하도록 충돌 구체를 루트로 만들고 몬스터 겹침 이벤트를 연결합니다.
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
		CollisionComponent->SetCollisionProfileName(ORCollisionProfiles::PlayerProjectile);
		CollisionComponent->SetGenerateOverlapEvents(true);
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleOverlap);
		SetRootComponent(CollisionComponent);
	}

	// 중력 없이 수평면에서만 이동하고, 벽에서는 속도를 잃지 않고 반사하도록 설정합니다.
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 1.0f;
		ProjectileMovementComponent->Friction = 0.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bConstrainToPlane = true;
		ProjectileMovementComponent->SetPlaneConstraintNormal(FVector::UpVector);
		ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &ThisClass::HandleBounce);
	}
}

void AORProjectile::InitializeProjectile(const UORWeaponData& WeaponData, const FGameplayEffectSpecHandle& InDamageSpec)
{
	// 명중 시 적용할 피해 효과와 남은 도탄 횟수를 보관합니다.
	DamageSpec = InDamageSpec;
	RemainingBounceCount = WeaponData.GetMaxBounceCount();

	// 탄환 반지름과 탄속을 적용하며, 생성 완료 시 이동 컴포넌트가 정면 방향 초기 속도를 계산합니다.
	CollisionComponent->SetSphereRadius(WeaponData.GetProjectileRadius());
	ProjectileMovementComponent->InitialSpeed = WeaponData.GetProjectileSpeed();
	ProjectileMovementComponent->MaxSpeed = WeaponData.GetProjectileSpeed();

	// 사거리만큼 이동하면 사라지도록 수명을 정하며, 사거리 제한이 없으면 최대 수명을 적용합니다.
	// BeginPlay에서 InitialLifeSpan으로 수명이 다시 설정되므로 생성 완료 전에 값을 지정합니다.
	const float Range = WeaponData.GetRange();
	InitialLifeSpan = Range > 0.0f ? Range / WeaponData.GetProjectileSpeed() : UnlimitedRangeLifeSpan;
}

void AORProjectile::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 같은 이동에서 여러 몬스터와 겹쳐도 한 번만 피해를 주도록 제거 중인 탄환은 처리하지 않습니다.
	if (IsActorBeingDestroyed())
	{
		return;
	}

	// 피해를 받을 수 있는 ASC가 있는 대상에게만 피해 효과를 적용합니다.
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	if (!TargetASC)
	{
		return;
	}
	TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpec.Data.Get());

	// 관통이 없으므로 첫 명중 대상에서 탄환을 제거합니다.
	Destroy();
}

void AORProjectile::HandleBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	// 남은 도탄 횟수가 있으면 한 번 차감하고 반사를 유지합니다.
	if (RemainingBounceCount > 0)
	{
		--RemainingBounceCount;
		return;
	}

	// 더 반사할 수 없으면 벽이나 장애물에 맞은 위치에서 탄환을 제거합니다.
	Destroy();
}
