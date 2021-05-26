#include "HealthSystem.h"
#include "HealthComponent.h"
#include "Entity.h"
#include "Game.h"
#include "Coordinator.h"
#include "EntityCreator.h"
//#include "ECS.h"

static std::vector<Entity> del_list;


void HealthSystem::update()
{
	for (auto e : m_entities)
	{

	}
}

void HealthSystem::delete_entites()
{
	while (del_list.size() > 0)
	{
		Game::coordinator->destroy_entity(del_list.back());
		del_list.pop_back();
	}
}

void HealthSystem::deal_damage(Entity e, HealthComponent& health, const float& damage)
{

#ifdef ECS_DEBUG
	std::cout << "Entity: " << e << " HP: " << health.health << " deal_damage: " << damage << std::endl;
#endif

	health.health -= damage;

	if (health.health < 0)
	{
		if (health.entity_type == ENTITY_TYPE_TILE)
		{
			Game::tileEntities[e] = 0;
		}
		++Game::entityDeaths[health.entity_type];
		del_list.push_back(e);


		return;
	}
	if (health.health > health.max_health)
	{
		health.health = health.max_health;
		return;
	}
}