#include <vector>
#include <SDL.h>
#include "Texture.h"
#include "Game.h"






std::vector<SDL_Texture*>* Texture::m_textures;

void Texture::init()
{
	m_textures = new std::vector<SDL_Texture*>;
	load_texture("resources/textures/tilesheet/TileSheet.bmp");


	load_texture("resources/textures/test/MakingMap2.bmp"); 


	load_texture("resources/textures/test/Player_placeholder.bmp");


	load_texture("resources/textures/mole/Mole_Sprite_TopDown_Flooded_SpriteSheet.bmp");














	std::cout << "textures initalized" << "\n";
}

void Texture::clean()
{
	delete m_textures;
}

void Texture::load_texture(const char* path)
{
	static SDL_Surface* loader_surface;
	SDL_Texture* texture;
	if (loader_surface = SDL_LoadBMP(path))
	{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		if (texture = SDL_CreateTextureFromSurface(Game::renderer, loader_surface))
		{
			m_textures->push_back(texture);
			if (m_textures->back() == NULL)
			{
				std::cout << "failed to load " << "\n";
			}
			SDL_FreeSurface(loader_surface);
			return;
		}
		else
		{
			std::cout << "texture failed to load" << std::endl;
		}
	}
	else
	{
		std::cout << "surface failed to load" << std::endl;
	}
	std::cout << "failed to load " << path << std::endl;
}

SDL_Texture* Texture::get_texture(  uint32_t pos)
{
	return m_textures->at(pos);
}