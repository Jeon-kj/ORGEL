#include "ORContactDamageComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "ORGameplayTags.h"
#include "Define/Define.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

UORContactDamageComponent::UORContactDamageComponent()
{
	// 플레이어와만 겹치고 이동과 내비게이션에는 영향을 주지 않는 판정 캡슐로 설정합니다.
	{
		SetCollisionProfileName(ORCollisionProfiles::EnemyContact);
		SetGenerateOverlapEvents(true);
		SetCanEverAffectNavigation(false);
	}

	// 플레이어와 닿고 떨어지는 시점을 받도록 겹침 이벤트를 연결합니다.
	{
		OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBeginOverlap);
		OnComponentEndOverlap.AddDynamic(this, &ThisClass::HandleEndOverlap);
	}
}

void UORContactDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	// 서로 막는 두 캡슐이 맞닿아도 겹치도록 소유 캐릭터 캡슐보다 여유값만큼만 크게 설정합니다.
	const UCapsuleComponent* OwnerCapsule = CastChecked<ACharacter>(GetOwner())->GetCapsuleComponent();
	SetCapsuleSize(OwnerCapsule->GetUnscaledCapsuleRadius() + Padding, OwnerCapsule->GetUnscaledCapsuleHalfHeight() + Padding);
}

void UORContactDamageComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 닿은 즉시 피해를 시도하고, 피격 무적이 끝난 뒤에도 닿아 있으면 다시 피해를 주도록 반복합니다.
	ContactTarget = OtherActor;
	ApplyContactDamage();
	GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &ThisClass::ApplyContactDamage, DamageInterval, true);
}

void UORContactDamageComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 현재 대상과 떨어졌을 때만 반복 시도를 중지합니다.
	if (OtherActor == ContactTarget.Get())
	{
		GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
		ContactTarget.Reset();
	}
}

void UORContactDamageComponent::ApplyContactDamage()
{
	// 무적 중에는 피해 거부 로그가 반복되지 않도록 효과 적용을 생략합니다.
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ContactTarget.Get());
	if (TargetASC->HasMatchingGameplayTag(ORGameplayTags::Status_Invulnerable))
	{
		return;
	}

	// 몬스터 ASC에서 접촉 피해량을 담은 피해 효과를 만들어 대상에게 적용합니다.
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	const FGameplayEffectSpecHandle DamageSpec = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, SourceASC->MakeEffectContext());
	DamageSpec.Data->SetSetByCallerMagnitude(ORGameplayTags::SetByCaller_Damage, Damage);
	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpec.Data.Get(), TargetASC);
}
