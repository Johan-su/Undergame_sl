#pragma once
#include "DebugMacros.h"
struct ProjectileComponent
{
	float damage;


#ifdef ECS_DEBUG
	 uint32_t entity;
#endif
};