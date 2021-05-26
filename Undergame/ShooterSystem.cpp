#include "DebugMacros.h"
#include "ShooterSystem.h"
#include "EntityCreator.h"
#include "ECS.h"


void ShooterSystem::update()
{
	for (const auto e : m_entities)
	{
		auto& pos = Game::coordinator->get_component<PositionComponent>(e);
		auto& size = Game::coordinator->get_component<SizeComponent>(e);
		auto& move = Game::coordinator->get_component<MovementComponent>(e);
		auto& shoot = Game::coordinator->get_component<ShooterComponent>(e);

#ifdef ECS_DEBUG
		SDL_assert(pos.entity == e);
		SDL_assert(size.entity == e);
		SDL_assert(move.entity == e);
		SDL_assert(shoot.entity == e);
#endif

		if (shoot.states[0])
		{
			++shoot.count;

			if (shoot.count == shoot.firingrate)
			{
				EntityCreator::create_entity(ENTITY_TYPE_BULLET, pos.pos.x + size.size.x / 2 + shoot.gunlength * cosf(move.angle), pos.pos.y + size.size.y / 2 + shoot.gunlength * sinf(move.angle), *(uint32_t*)(&move.angle));
				shoot.count = 0;
			}
			//for(int i = 0; i < 10; ++i)
		}
	}
}