#pragma once

#include "Engine/EngineTypes.h"

namespace ORCollisionChannels
{
	/** 상호작용 가능한 오브젝트를 탐색하는 전용 Trace Channel을 지정합니다. */
	inline constexpr ECollisionChannel Interaction = ECC_GameTraceChannel1;
	
	/** 플레이어를 식별하는 전용 Object Channel을 지정합니다. */
	inline constexpr ECollisionChannel Player = ECC_GameTraceChannel2;

	/** 적을 식별하는 전용 Object Channel을 지정합니다. */
	inline constexpr ECollisionChannel Enemy = ECC_GameTraceChannel3;
}

namespace ORCollisionProfiles
{
	/** 플레이어 캡슐에 사용할 충돌 프로필 이름을 지정합니다. */
	inline const FName Player = TEXT("Player");

	/** 몬스터 캡슐에 사용할 충돌 프로필 이름을 지정합니다. */
	inline const FName Enemy = TEXT("Enemy");

	/** 플레이어 탄환에 사용할 충돌 프로필 이름을 지정합니다. */
	inline const FName PlayerProjectile = TEXT("PlayerProjectile");
}