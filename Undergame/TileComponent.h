#pragma once
#include <cstdint>
#include "DebugMacros.h"
#include "vecf.h"
struct TileComponent
{
#ifdef ECS_DEBUG
	 uint32_t entity;
#endif
};