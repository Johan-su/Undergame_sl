#pragma once
#include <memory>
#include "ECS.h"

class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> register_system()
	{
		std::shared_ptr<T> system = std::make_shared<T>();
		std::string systemName = typeid(system).name();
		m_systemMap[systemName] = system;
		return system;

	}
	template<typename T>
	void set_signature(T system, std::bitset<MAX_COMPONENTS> signature)
	{
		std::string systemName = typeid(T).name();

		m_signatures[systemName] = signature;
	}
	void remove_entity(const Entity& e)
	{
		for (auto const& pair : m_systemMap)
		{
			auto const& system = pair.second;
			system->m_entities.erase(e);
		}
	}
	void entity_changed_signature(const Entity& e, std::bitset<MAX_COMPONENTS> e_signature)
	{
		for (auto const& pair : m_systemMap)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto& signature = m_signatures[type];
			auto const& comparisionSignature = signature & e_signature;

			if (comparisionSignature == signature)
			{
				system->m_entities.insert(e);
			}
			else
			{
				system->m_entities.erase(e);
			}
		}
	}
private:

	std::unordered_map<std::string, std::bitset<MAX_COMPONENTS>> m_signatures;

	std::unordered_map<std::string, std::shared_ptr<System>> m_systemMap;
};
