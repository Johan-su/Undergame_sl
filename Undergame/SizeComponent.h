#pragma once
#include "DebugMacros.h"
#include "vecf.h"
struct SizeComponent
{
	Vec2f size;



#ifdef ECS_DEBUG
	 uint32_t entity;
#endif
};