#pragma once
#include <memory>
#include "DebugMacros.h"
#include "ECS.h"

class ProjectileSystem : public System
{
public:

	void init(std::shared_ptr<HealthSystem> hs);

	void clean();

	void update();

private:

	std::shared_ptr<HealthSystem> m_hs;
};