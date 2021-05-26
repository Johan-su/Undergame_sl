#pragma once
#include <memory>
#include "DebugMacros.h"
#include "HealthComponent.h"
#include "System.h"
#include "Entity.h"
//#include "ECS.h"

class HealthSystem : public System
{
public:

	void update();

	void delete_entites();

	void deal_damage(Entity e, HealthComponent& health, const float& damage);

private:





private:
};