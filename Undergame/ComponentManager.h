#pragma once
#include <string>
#include <array>
#include <memory>
#include <unordered_map>
#include "ECS.h"
class ComponentManager
{
public:
	ComponentManager()
	: m_componentTypeCount(0), m_arrayPointers({})
	{
	}

	void clean()
	{
		for (int i = 0; i < m_arrayPointers.size(); ++i)
		{
			delete m_arrayPointers[i];
		}
	}
	template<typename T>
	void register_component_type()
	{
		if (m_componentTypeCount >= MAX_COMPONENTS)
		{
			throw "too many component types";
			return;
		}
		std::string componentName = typeid(T).name();
		for (int i = 0; i < m_componentTypeCount; ++i)
		{
			if (m_typeToArray.find(componentName) != m_typeToArray.end())
			{
				throw "component already exists";
			}
		}
		m_typeToArray[componentName] = m_componentTypeCount;
		m_arrayPointers[m_componentTypeCount] = new ComponentArray<T>();
		++m_componentTypeCount;
	}
	template<typename T>
	void add_component(Entity e, T component)
	{
		std::string componentName = typeid(T).name();

		get_component_array<T>()->add_component(e, component);

	}
	template<typename T>
	void destroy_component(Entity e)
	{
		get_component_array<T>()->destroy_component(e);
	}
	void remove_entity(Entity e)
	{
		for (unsigned i = 0; i < m_componentTypeCount; ++i)
		{
			m_arrayPointers[i]->destroy_entity(e);
		}
	}
	template<typename T>
	ComponentArray<T>* get_component_array()
	{
		return static_cast<ComponentArray<T>*>(m_arrayPointers[m_typeToArray[typeid(T).name()]]);
	}

	template<typename T>
	uint8_t get_component_pos()
	{
		std::string componentName = typeid(T).name();
		return m_typeToArray[componentName];
	}

	template<typename T>
	T& get_component(Entity e)
	{
#ifdef ECS_DEBUG
		SDL_assert(e != 0xFFFFFFFF);
#endif
		return get_component_array<T>()->get_component(e);
	}




private:

	std::unordered_map<std::string, uint8_t> m_typeToArray;
	std::array<IComponentArray*, MAX_COMPONENTS> m_arrayPointers;
	uint8_t m_componentTypeCount;
};