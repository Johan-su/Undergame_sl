#pragma once
#include <memory>
#include "ECS.h"

class Coordinator
{
public:
	void test()
	{
		if (m_componentManager == nullptr || m_systemManager == nullptr || m_entityManager == nullptr) throw "nullptr";
		std::cout << "test executed successfully" << std::endl;
	}
	void init()
	{
		m_entityManager = new EntityManager();
		m_componentManager = new ComponentManager();
		m_systemManager = new SystemManager();
	}
	void clean()
	{
		delete m_entityManager;;
		delete m_componentManager;
		delete m_systemManager;
	}
	Entity create_entity() // should not be called explicitly, use EntityCreator.
	{
		return m_entityManager->Create_entity();
	}
	void destroy_entity(Entity e) 
	{
#ifdef ECS_DEBUG
		std::cout << "Entity: " << e << " Destroyed" << std::endl;
#endif

		m_systemManager->remove_entity(e);
		m_componentManager->remove_entity(e);
		m_entityManager->destroy_entity(e);
	}
	template<typename T>
	void register_component()
	{
		m_componentManager->register_component_type<T>();
	}
	template<typename T>
	void add_component(Entity e, T component)
	{
		m_componentManager->add_component<T>(e, component);
		auto& sig = m_entityManager->Get_signature(e);
		sig.set(m_componentManager->get_component_pos<T>(), true);
		m_systemManager->entity_changed_signature(e, sig);
		//m_entityManager->Set_signature(e, sig);
	}
	template<typename T>
	void remove_component(Entity e)
	{
		auto sig = m_entityManager->Get_signature(e);
		m_componentManager->destroy_component<T>(e);
		sig.set(m_componentManager->get_component_pos<T>(), false);
		m_entityManager->Set_signature(e, sig);
		m_systemManager->entity_changed_signature(e, sig);
	}
	template<typename T>
	T& get_component(Entity e)
	{

		return m_componentManager->get_component<T>(e);
	}
	template<typename T>
	uint8_t get_signature_pos()
	{
		return m_componentManager->get_component_pos<T>();
	}
	template<typename T>
	std::shared_ptr<T> register_system()
	{
		return m_systemManager->register_system<T>();
	}
	template<typename T>
	void set_signature(T system, std::bitset<MAX_COMPONENTS> signature)
	{
		m_systemManager->set_signature(system, signature);
	}
	std::bitset<MAX_COMPONENTS> get_signature(Entity e)
	{
		return m_entityManager->Get_signature(e);
	}

private:
	EntityManager* m_entityManager;
	ComponentManager* m_componentManager;
	SystemManager* m_systemManager;
};