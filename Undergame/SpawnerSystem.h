#pragma once
#include <random>
#include "ECS.h"

class SpawnerSystem : public System
{
public:

	void init();

	void clean();

	void update();



private:

	bool inside_map(float x, float y);

	uint32_t spawntimer, spawnrate, iterations;
};