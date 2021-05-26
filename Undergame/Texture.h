#pragma once
#include <memory>
#include <vector>
#include <SDL.h>

enum TEXTURE
{
	TEXTURE_TILESHEET,
	TEXTURE_BULLET,
	TEXTURE_PLAYER,
	TEXTURE_MOLE
};

class Texture
{
public:

	static void init();

	static void clean();

	static SDL_Texture* get_texture(  uint32_t pos);

private:
	static void load_texture(const char* path);

	static std::vector<SDL_Texture*>* m_textures;


};