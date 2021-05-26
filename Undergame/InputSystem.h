#pragma once
#include <SDL.h>
#include "DebugMacros.h"
#include "Game.h"
#include "ECS.h"
class InputSystem : public System
{
public:
	static void update();

	static void mouse_pos();

	static void mouse_event_down();

	static void mouse_event_up();

	static void key_event_down();

	static void key_event_up();

	static bool buttonStates[7];

	static int mx, my;

private:

};