#pragma once
#include <vector>
#include "DebugMacros.h"

enum AI_STATES
{
	AI_STATE_RANDOM_WALKING,
	AI_STATE_TRACK_LAST_KNOWN,
	AI_STATE_TRACKING,
};


struct AiComponent
{
	float trackRadius;
	float detectionRadius;
	float lastX;
	float lastY;
	float damage;
	std::vector<uint32_t> path_list;
	uint16_t id;
	uint8_t state;




#ifdef ECS_DEBUG
	 uint32_t entity;
#endif
};