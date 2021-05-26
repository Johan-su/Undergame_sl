#pragma once
#include <SDL.h>
#include "DebugMacros.h"
#include "ECS.h"
class PlayerSystem : public System
{
public:

	void update();

private:

	void set_Camera_to_player(const PositionComponent& pc);

};