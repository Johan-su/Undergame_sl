#pragma once
#include "DebugMacros.h"
struct ShooterComponent
{
	float gunlength;
	bool states[4];
	uint16_t count;
	uint16_t firingrate; // lower is faster



#ifdef ECS_DEBUG
	 uint32_t entity;
#endif
};