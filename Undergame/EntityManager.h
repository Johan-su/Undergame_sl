#pragma once
#include <array>
#include <bitset>
#include <queue>
#include "SDL.h"
#include "Entity.h"
#include "ECS.h"


class EntityManager
{
public:
	EntityManager()
	: m_entityCount(0)
	{
		for (uint32_t id = 0; id < MAX_ENTITIES; ++id)
		{
			m_AvailableIDs.push(id);
		}
	}

	Entity Create_entity()
	{
		SDL_assert(m_AvailableIDs.size() > 0);

		Entity e = m_AvailableIDs.front();
		m_AvailableIDs.pop();
		++m_entityCount;
		return e;
	}

	void destroy_entity(const Entity& e)
	{
		m_signatures[e].reset();

		m_AvailableIDs.push(e);
		--m_entityCount;
	}

	void Set_signature(const Entity& e, std::bitset<MAX_COMPONENTS> signature)
	{
		m_signatures[e] = signature;
	}

	std::bitset<MAX_COMPONENTS>& Get_signature(const Entity& e)
	{
		return m_signatures[e];
	}

private:
	std::queue<uint32_t> m_AvailableIDs;

	std::array<std::bitset<MAX_COMPONENTS>, MAX_ENTITIES> m_signatures;

	uint32_t m_entityCount;
};