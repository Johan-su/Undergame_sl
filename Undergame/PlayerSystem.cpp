#include "DebugMacros.h"
#include "Game.h"
#include "ECS.h"
#include <cmath>
#include "PlayerSystem.h"

void PlayerSystem::update()
{
#ifdef PRINT_DEBUG
	static int count = 0;
#endif
	if (m_entities.size() == 0)
	{
		Game::Running = false;
	}
	for (auto e : m_entities)
	{

		auto& playc = Game::coordinator->get_component<PlayerComponent>(e);
		auto& size = Game::coordinator->get_component<SizeComponent>(e);
		auto& movec = Game::coordinator->get_component<MovementComponent>(e);
		auto& pc = Game::coordinator->get_component<PositionComponent>(e);
		auto& shoot = Game::coordinator->get_component<ShooterComponent>(e);
		auto& digger = Game::coordinator->get_component<DiggerComponent>(e);


/*TEMP*/
		//auto& health = Game::coordinator->get_component<HealthComponent>(e);
		//std::cout << "health: " << health.health << std::endl;
		//
#ifdef ECS_DEBUG
		SDL_assert(playc.entity == e);
		SDL_assert(size.entity == e);
		SDL_assert(movec.entity == e);
		SDL_assert(pc.entity == e);
		SDL_assert(shoot.entity == e);
#endif

		set_Camera_to_player(pc);
		movec.angle = atanf((SCREEN_HEIGHT / 2 + size.size.y / 2 - (float)(InputSystem::my)) / (SCREEN_WIDTH / 2 + size.size.x / 2 - (float)(InputSystem::mx)));

		if ((size.size.x + SCREEN_WIDTH) / 2 - InputSystem::mx >= 0)
		{
			movec.angle -= 3.14159265359f; // pi
		}

		movec.angle = fmod(movec.angle + 6.28318530718f, 6.28318530718f); // 6.28318530718 == 2pi

		//std::cout << movec.angle << std::endl;

		movec.velocity.x = 0;
		movec.velocity.y = 0;
		digger.drillState = 0;

#ifdef PRINT_DEBUG
		if (count == 60)
		{
			std::cout << InputSystem::mx << " mx my " << InputSystem::my << std::endl;
			std::cout << "angle " << movec.angle << std::endl;
			std::cout << pc.pos.x << " px py " << pc.pos.y << std::endl;
			count = 0;
		}
#endif
		if (InputSystem::buttonStates[0]) // w
		{
			movec.velocity.y = -1;
		}
		if (InputSystem::buttonStates[1]) // a
		{
			movec.velocity.x = -1;
		}
		if (InputSystem::buttonStates[2]) // s
		{
			movec.velocity.y = 1;
		}
		if (InputSystem::buttonStates[3]) // d
		{
			movec.velocity.x = 1;
		}
		if (InputSystem::buttonStates[4] && playc.bullets > 0) // mouse left
		{
			shoot.states[0] = true;
		}
		else
		{
			shoot.states[0] = false;
		}
		if (InputSystem::buttonStates[5]) // mouse middle
		{

		}
		if (InputSystem::buttonStates[6]) // mouse right
		{
			digger.drillState = 1;
		}
	}

#ifdef PRINT_DEBUG
	++count;
#endif
}
void PlayerSystem::set_Camera_to_player(const PositionComponent& pc)
{
	Game::offsetx = static_cast<int>(pc.pos.x - static_cast<float>(SCREEN_WIDTH) / 2);
	Game::offsety = static_cast<int>(pc.pos.y - static_cast<float>(SCREEN_HEIGHT) / 2);
}