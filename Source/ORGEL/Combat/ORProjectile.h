#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "ORProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

USTRUCT()
struct FORProjectileParams
{
	GENERATED_BODY()

	/** 탄환의 충돌 반지름을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|탄환", meta = (ClampMin = "0.1", UIMin = "0.1", Units = "cm"))
	float Radius = 10.0f;

	/** 탄환의 이동 속도를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|탄환", meta = (ClampMin = "1.0", UIMin = "1.0", Units = "cm/s"))
	float Speed = 1500.0f;

	/** 탄환 사거리를 지정하며, 0이면 사거리 제한이 없습니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|탄환", meta = (ClampMin = "0.0", UIMin = "0.0", Units = "cm"))
	float Range = 0.0f;

	/** 탄환이 벽에서 반사될 수 있는 최대 횟수를 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|탄환", meta = (ClampMin = "0", UIMin = "0"))
	int32 MaxBounceCount = 0;
};

UCLASS()
class ORGEL_API AORProjectile : public AActor
{
	GENERATED_BODY()

// ─────────────────────────────────────────────────────────────
// Lifecycle
// ─────────────────────────────────────────────────────────────
public:
	/** 플레이어 탄환 충돌 구체와 중력 없이 반사하는 이동 컴포넌트를 생성하고 명중 이벤트를 연결합니다. */
	AORProjectile();


// ─────────────────────────────────────────────────────────────
// Initialization
// ─────────────────────────────────────────────────────────────
public:
	/** 탄환 수치로 반지름, 탄속, 도탄 횟수, 수명을 설정하고 명중 시 적용할 피해 효과를 보관합니다. 지연 생성 후 FinishSpawning 전에 호출합니다. */
	void InitializeProjectile(const FORProjectileParams& Params, const FGameplayEffectSpecHandle& InDamageSpec);


// ─────────────────────────────────────────────────────────────
// Hit
// ─────────────────────────────────────────────────────────────
private:
	/** 겹친 몬스터에게 피해 효과를 적용하고 탄환을 제거합니다. */
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 벽이나 장애물에 반사되면 남은 도탄 횟수를 차감하고, 남은 횟수가 없으면 탄환을 제거합니다. */
	UFUNCTION()
	void HandleBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);


// ─────────────────────────────────────────────────────────────
// Components
// ─────────────────────────────────────────────────────────────
private:
	/** 벽과 장애물에는 막히고 몬스터와는 겹치는 탄환 충돌 구체를 참조합니다. */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComponent;

	/** 탄환의 직선 이동과 벽 반사를 처리하는 이동 컴포넌트를 참조합니다. */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;


// ─────────────────────────────────────────────────────────────
// Projectile Variables
// ─────────────────────────────────────────────────────────────
private:
	/** 사거리 제한이 없는 무기의 탄환이 무한히 남지 않도록 적용할 최대 수명을 지정합니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|탄환", meta = (ClampMin = "0.1", UIMin = "0.1", Units = "s"))
	float UnlimitedRangeLifeSpan = 10.0f;

	/** 명중한 몬스터에게 적용할 피해 효과 명세를 보관합니다. */
	FGameplayEffectSpecHandle DamageSpec;

	/** 벽이나 장애물에서 더 반사할 수 있는 남은 횟수를 보관합니다. */
	int32 RemainingBounceCount = 0;
};
