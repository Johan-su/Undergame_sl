#pragma once
#include <cstdlib>
#include "DebugMacros.h"

struct HealthComponent
{
	float health, max_health;
	uint8_t entity_type;


#ifdef ECS_DEBUG
	unsigned int entity;
#endif
};