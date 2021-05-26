#pragma once
#include <functional>
#include <array>
#include <SDL.h>
#include "DebugMacros.h"
#include "ECS.h"
#include "Tilemap.h"

class StaticRenderSystem : public System
{
public:


	static void render_tiles(int offx, int offy);

	static void render_tile(Uint16 e, int offx, int offy);

	static void init();

private:
	static std::vector<std::function<void(float x, float y, int offx, int offy)>> func_pointers;
	static std::vector<SDL_Texture*> tile_textures;
};
