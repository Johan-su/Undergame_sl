#pragma once
#include "System.h"
#include "Entity.h"
#include "vecf.h"

class TargetingSystem : public System
{
public:

	Entity nearest_player(float x, float y);

	float nearest_player_distance(float x, float y); //TODO: if needed for performance changed float x, float y to vec2f

	Vec2f nearest_player_pos(float x, float y);

	Vec2f nearest_player_size(float x, float y);

};