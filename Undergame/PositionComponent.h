#pragma once
#include "DebugMacros.h"
#include "vecf.h"
struct PositionComponent
{
	Vec2f pos;



#ifdef ECS_DEBUG
	 uint32_t entity;
#endif
};