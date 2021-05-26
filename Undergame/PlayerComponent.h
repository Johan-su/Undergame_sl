#pragma once
#include <cstdint>
#include "DebugMacros.h"

struct PlayerComponent
{
	uint32_t bullets;

	uint8_t bulletWeaponsType; // bullet weapons



#ifdef ECS_DEBUG
	 uint32_t entity;
#endif
};