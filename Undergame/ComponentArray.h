#pragma once
#include <iostream>
#include "ECS.h"

class IComponentArray
{
public:
	virtual void destroy_entity(Entity e) = 0;

};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	ComponentArray()
		: m_size(0)
	{
		for (unsigned int i = 0; i < MAX_ENTITIES; ++i)
		{
			entity_to_index[i] = 0xFFFFFFFF;
			index_to_entity[i] = 0xFFFFFFFF;
		}
	}
	void add_component(Entity e, T component)
	{
		m_data[m_size] = component;
		entity_to_index[e] = m_size;
		index_to_entity[m_size] = e;
		++m_size;
	}
	void destroy_component(Entity e)
	{
#ifdef ECS_DEBUG
		SDL_assert(entity_to_index[e] < m_size);
#endif
		--m_size;
		if (entity_to_index[e] == m_size) // if at the end of array remove it.
		{
			index_to_entity[m_size] = 0xFFFFFFFF;
			entity_to_index[e] = 0xFFFFFFFF;
			return;
		}


		m_data[entity_to_index[e]] = m_data[m_size]; // moves component from last element, to newly destroyed.

		entity_to_index[index_to_entity[m_size]] = entity_to_index[e]; // change last Entity's index to the replaced position.
		
		index_to_entity[entity_to_index[e]] = index_to_entity[m_size];

		entity_to_index[e] = 0xFFFFFFFF; // removes unused index.

		index_to_entity[m_size] = 0xFFFFFFFF; // removes unused index.
	}
	void destroy_entity(Entity e) override
	{
		if (entity_to_index[e] < m_size)
		{
			destroy_component(e);
		}
	}
	T& get_component(Entity e)
	{
#ifdef ECS_DEBUG
		SDL_assert(entity_to_index[e] != 0xFFFFFFFF);
#endif
		return m_data[entity_to_index[e]]; 
	}

	  uint32_t get_size() const
	{
		return m_size;
	}

private:
	std::array<T, MAX_ENTITIES> m_data;
	std::array<  uint32_t, MAX_ENTITIES> entity_to_index;
	std::array< uint32_t, MAX_ENTITIES> index_to_entity;
	uint32_t m_size;
};