#include <iostream>
#include <SDL.h>
#include "DebugMacros.h"
#include "Texture.h"
#include "ECS.h"
#include "StaticRenderSystem.h"


std::vector<std::function<void(float x, float y, int offx, int offy)>> StaticRenderSystem::func_pointers;
std::vector<SDL_Texture*> StaticRenderSystem::tile_textures;


void StaticRenderSystem::render_tiles(int offx, int offy)
{
	int x0 = offx - 2 * TILE_SIZE; // top left X value corner of the window + extra
	int y0 = offy - 2 * TILE_SIZE; // top left Y value corner of the window + extra

	int x1 = offx + SCREEN_WIDTH + 2 * TILE_SIZE; // bottom right X value corner of the window + extra
	int y1 = offy + SCREEN_HEIGHT + 2 * TILE_SIZE; // bottom right Y value corner of the window + extra


	uint16_t gidtl = MAP_SIZE * (y0 / TILE_SIZE) + x0 / TILE_SIZE; // id of top left tile

	for (uint32_t y = 0; y < (SCREEN_HEIGHT + 4 * TILE_SIZE) / TILE_SIZE; ++y)
	{
		for (uint32_t x = 0; x < (SCREEN_WIDTH + 4 * TILE_SIZE) / TILE_SIZE; ++x)
		{
			uint16_t id = gidtl + x + y * MAP_SIZE; 
			render_tile(id, offx, offy);
		} 
	}
}
void StaticRenderSystem::render_tile(uint16_t e, int offx, int offy)
{

	auto type = Game::tileEntities[e];
	if (type == 0)
	{
		return;
	}


	float x = static_cast<float>(TILE_SIZE * (e % MAP_SIZE));
	float y = static_cast<float>(TILE_SIZE * (e / MAP_SIZE));

	const auto& func = func_pointers[type];
	func(x, y, offx, offy);

}

void StaticRenderSystem::init()
{
	tile_textures.push_back(Texture::get_texture(0));

	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 0, 0, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 0, 0, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) })); // 0
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 64, 0, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &(SDL_Rect({ 64, 0, 64, 64 })), &(SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) })));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 128, 0, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 128, 0, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 192, 0, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 192, 0, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 256, 0, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 256, 0, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 0, 64, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 0, 64, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 64, 64, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 64, 64, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 128, 64, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 128, 64, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});

	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 192, 64, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 192, 64, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 256, 64, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 256, 64, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 0, 128, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 0, 128, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 64, 128, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 64, 128, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 128, 128, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 128, 128, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 192, 128, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 192, 128, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) })); // 13
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 256, 128, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 256, 128, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 0, 192, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 0, 192, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 64, 192, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 64, 192, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 128, 192, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 128, 192, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 192, 192, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 192, 192, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});


	func_pointers.push_back([&](float x, float y, int offx, int offy)
	{
		auto a = SDL_Rect({ 256, 192, 64, 64 });
		auto b = SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
		SDL_RenderCopyF(Game::renderer, tile_textures[0], &a, &b);
		//SDL_RenderCopyF(Game::renderer, tile_textures[0], &SDL_Rect({ 256, 192, 64, 64 }), &SDL_FRect({ x - offx, y - offy, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) }));
	});
	
}

