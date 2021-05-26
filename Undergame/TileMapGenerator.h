#pragma once
#include "ECS.h"
#include "TileMap.h"
class TileMapGenerator
{
public:

	static void init( uint32_t Seed);

	static TileMap* create_map_random();

	static TileMap* create_map_value();

	static TileMap* create_map_perlin();

	static TileMap* create_map_simplex();

	static void entities_from_map(TileMap* tm);


private:
	static void create_boundary(TileMap* tm);

	static float octaveValue(float x, float y, uint16_t octaves, float persistance);

	static float value2d(float x, float y);

	static float create_gradient_value(float x, float y);

	static float octavePerlin(float x, float y, uint16_t octaves, float persistance);

	static float perlin2d(float x, float y);

	static Vec2f create_gradient_vector(float x, float y);

	static Vec2f create_direction_vector(float x, float y, const Vec2f& vec);

	static float lerp(float x, float y, float weight);

	static float fade(float a);

	static float dotProduct(const Vec2f& l, const Vec2f& r);
};