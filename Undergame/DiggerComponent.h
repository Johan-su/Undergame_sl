#pragma once
#include <cstdint>
#include <cstdlib>
#include "ECS.h"

struct DiggerComponent
{
	uint8_t drillLVL;
	uint8_t drillState;




#ifdef ECS_DEBUG
	uint32_t entity;
#endif
};