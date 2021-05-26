#pragma once
#include <cstdint>
#include <array>
#include "ECS.h"
struct TileMap
{
	std::array<uint8_t, MAP_SIZE * MAP_SIZE> grid;
};