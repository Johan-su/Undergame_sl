#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <SDL.h>
#include "ECS.h"

enum EntityTypes
{
	ENTITY_TYPE_PLAYER,
	ENTITY_TYPE_NPC,
	ENTITY_TYPE_TILE,
	ENTITY_TYPE_MOLE,
	ENTITY_TYPE_BULLET,
	ENTITY_TYPE_TEXTBOX,
	ENTITY_TYPE_STATIC_TEXTBOX
};
enum TileTypes
{
	TILE_TYPE_AIR,
	TILE_TYPE_STONE,
	TILE_TYPE_DIRT,
	TILE_TYPE_WOOD,
	TILE_TYPE_STONE_BRICK,
	TILE_TYPE_RUINED_STONE,
	TILE_TYPE_SAND,
	TILE_TYPE_GRAVEL,
	TILE_TYPE_MOSSY_STONE_BRICK,
	TILE_TYPE_GOLD,
	TILE_TYPE_COAL,
	TILE_TYPE_IRON,
	TILE_TYPE_LAPIS,
	TILE_TYPE_EMERALD,
	TILE_TYPE_DIAMOND,
	TILE_TYPE_REDSTONE

};



class EntityCreator
{
public:
	static void init();

	static void create_entity(const size_t& type, const float& x, const float& y, uint32_t data);

private:
	static std::vector<std::function<void(const Entity&, const float&, const float&, uint32_t)>> func_pointers;

	static unsigned int create_player_id();

};
