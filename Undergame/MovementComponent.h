#pragma once
#include "DebugMacros.h"
#include "vecf.h"

struct MovementComponent
{
	Vec2f velocity;
	float speed;
	float angle;



#ifdef ECS_DEBUG
	 uint32_t entity;
#endif
};