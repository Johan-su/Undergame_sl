#pragma once
#include "System.h"
#include "Entity.h"
#include "PositionComponent.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include "SizeComponent.h"
class DiggerSystem : public System
{
public:

	void init(std::shared_ptr<HealthSystem> hs);

	void update();


private:

	std::shared_ptr<HealthSystem> m_hs;

	bool is_facing_tile(Entity tile, PositionComponent& facersPos, SizeComponent& facersSize, MovementComponent& facersMove);

};