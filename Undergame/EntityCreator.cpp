#include <functional>
#include "DebugMacros.h"
#include "SDL.h"
#include "Texture.h"
#include "ECS.h"
#include "Game.h"
#include "EntityCreator.h"


std::vector<std::function<void(const Entity& ,const float&, const float&, uint32_t data)>> EntityCreator::func_pointers;



void EntityCreator::init()
{
	func_pointers.push_back([](const Entity& e, const float& x, const float& y, uint32_t data) // Playercontrolled
		{
			auto positionc = PositionComponent();
			auto cc = ColliderComponent();

			auto sc = SizeComponent();
			auto rc = RenderComponent();

			auto pc = PlayerComponent();

			auto hc = HealthComponent();
			auto mc = MovementComponent();

			auto shc = ShooterComponent();
			auto dc = DiggerComponent();

#ifdef ECS_DEBUG
			positionc.entity = e;
			cc.entity = e;

			sc.entity = e;
			rc.entity = e;

			pc.entity = e;

			hc.entity = e;
			mc.entity = e;

			shc.entity = e;
			dc.entity = e;
#endif

			positionc.pos.x = x;
			positionc.pos.y = y;

			cc.tile_id = 0xFFFFFFFF;
			cc.other_entity = 0xFFFFFFFF;

			sc.size.x = 24;
			sc.size.y = 24; 

			rc.src_rect = {0, 0, 92, 136}; // TODO: determine texture

			rc.texture = Texture::get_texture(2); // TODO: determine texture

			//pc.id = create_player_id();

			pc.bullets = 99999999; 
			pc.bulletWeaponsType = 0;

			hc.entity_type = ENTITY_TYPE_PLAYER;
			hc.max_health = 100.0f;
			hc.health = hc.max_health;

			mc.speed = 3.0f;

			shc.gunlength = 34;
			shc.count = 0;
			shc.firingrate = 4; // lower is faster
			dc.drillLVL = 0;

			Game::coordinator->add_component<PositionComponent>(e, positionc);
			Game::coordinator->add_component<ColliderComponent>(e, cc);

			Game::coordinator->add_component<SizeComponent>(e, sc);
			Game::coordinator->add_component<RenderComponent>(e, rc);

			Game::coordinator->add_component<PlayerComponent>(e, pc);

			Game::coordinator->add_component<HealthComponent>(e, hc);
			Game::coordinator->add_component<MovementComponent>(e, mc);

			Game::coordinator->add_component<ShooterComponent>(e, shc);
			Game::coordinator->add_component<DiggerComponent>(e, dc);


		});
	func_pointers.push_back([](const Entity& e, const float& x, const float& y, uint32_t data) // NPC (unused)
		{
		});
	func_pointers.push_back([](const Entity& e, const float& x, const float& y, uint32_t data) // tile
		{
			auto positionc = PositionComponent();
			auto tc = TileComponent();
			auto sc = SizeComponent();
			auto hc = HealthComponent();

#ifdef ECS_DEBUG
			positionc.entity = e;
			tc.entity = e;

			sc.entity = e;
			hc.entity = e;
#endif

			positionc.pos.x = x;
			positionc.pos.y = y;

			switch (data) // setting tilehealth
			{
			case TILE_TYPE_STONE:
				hc.max_health = 500.0f;
				break;
			case TILE_TYPE_DIRT:
				hc.max_health = 100.0f;
				break;
			case TILE_TYPE_WOOD:
				hc.max_health = 100.0f;
				break;
			case TILE_TYPE_STONE_BRICK:
				hc.max_health = 150.0f;
				break;
			case TILE_TYPE_RUINED_STONE:
				hc.max_health = 125.0f;
				break;
			case TILE_TYPE_SAND:
				hc.max_health = 100.0f;
				break;
			case TILE_TYPE_GRAVEL:
				hc.max_health = 100.0f;
				break;
			case TILE_TYPE_MOSSY_STONE_BRICK:
				hc.max_health = 150.0f;
				break;
			case TILE_TYPE_GOLD:
				hc.max_health = 150.0f;
				break;
			case TILE_TYPE_COAL:
				hc.max_health = 150.0f;
				break;
			case TILE_TYPE_IRON:
				hc.max_health = 150.0f;
				break;
			case TILE_TYPE_LAPIS:
				hc.max_health = 150.0f;
				break;
			case TILE_TYPE_EMERALD:
				hc.max_health = 150.0f;
				break;
			case TILE_TYPE_DIAMOND:
				hc.max_health = 150.0f;
				break;
			case TILE_TYPE_REDSTONE:
				hc.max_health = 150.0f;
				break;
			case 16:
				hc.max_health = 3E18f;
				break;
			default:
			hc.max_health = 100.0f;
				break;
			}
			hc.entity_type = ENTITY_TYPE_TILE;

			sc.size.x = TILE_SIZE;
			sc.size.y = TILE_SIZE;


			hc.health = hc.max_health;



			Game::coordinator->add_component<PositionComponent>(e, positionc);
			Game::coordinator->add_component<TileComponent>(e, tc);
			Game::coordinator->add_component<SizeComponent>(e, sc);
			Game::coordinator->add_component<HealthComponent>(e, hc);

		});
	func_pointers.push_back([](const Entity& e, const float& x, const float& y, uint32_t data) // mole 
		{
			auto positionc = PositionComponent();
			auto cc = ColliderComponent();

			auto sc = SizeComponent();
			auto rc = RenderComponent();

			auto hc = HealthComponent();
			auto mc = MovementComponent();

			auto ai = AiComponent();
			auto digger = DiggerComponent();

#ifdef ECS_DEBUG
			positionc.entity = e;
			cc.entity = e;

			sc.entity = e;
			rc.entity = e;

			hc.entity = e;
			mc.entity = e;

			ai.entity = e;
			digger.entity = e;
#endif

			positionc.pos.x = x;
			positionc.pos.y = y;

			cc.tile_id = 0xFFFFFFFF;
			cc.other_entity = 0xFFFFFFFF;

			hc.entity_type = ENTITY_TYPE_MOLE;
			hc.max_health = 70.0f;
			hc.health = hc.max_health;

			sc.size.x = 30.0f;
			sc.size.y = 30.0f;

			rc.src_rect = { 0, 0, 400, 503 };

			rc.texture = Texture::get_texture(TEXTURE_MOLE); 

			mc.speed = 1.0f + (1.0f + (float)(std::rand()) / (float)RAND_MAX);

			ai.trackRadius = (float)(TILE_SIZE) * 1.5f;
			ai.detectionRadius = (float)(TILE_SIZE) * 6.0f;
			ai.damage = 30.0f;
			ai.id = ((int)x / TILE_SIZE) + ((int)y / TILE_SIZE) * MAP_SIZE;;
			ai.state = 0;

			digger.drillState = 1;

			Game::coordinator->add_component<PositionComponent>(e, positionc);
			Game::coordinator->add_component<ColliderComponent>(e, cc);

			Game::coordinator->add_component<SizeComponent>(e, sc);
			Game::coordinator->add_component<RenderComponent>(e, rc);

			Game::coordinator->add_component<HealthComponent>(e, hc);
			Game::coordinator->add_component<MovementComponent>(e, mc);

			Game::coordinator->add_component<AiComponent>(e, ai);
			Game::coordinator->add_component<DiggerComponent>(e, digger);

		});
	func_pointers.push_back([](const Entity& e, const float& x, const float& y, uint32_t data) // bullet
		{
			auto positionc = PositionComponent();
			auto cc = ColliderComponent();

			auto sc = SizeComponent();
			auto rc = RenderComponent();

			auto mc = MovementComponent();
			auto pc = ProjectileComponent();

			auto health = HealthComponent();

#ifdef ECS_DEBUG
			positionc.entity = e;
			cc.entity = e;

			sc.entity = e;
			rc.entity = e;

			mc.entity = e;
			pc.entity = e;

			health.entity = e;
#endif

			positionc.pos.x = x;
			positionc.pos.y = y;

			cc.tile_id = 0xFFFFFFFF;
			cc.other_entity = 0xFFFFFFFF;

			sc.size.x = 4.0f;
			sc.size.y = 4.0f;

			rc.src_rect = { 0, 0, 677, 320 }; 

			rc.texture = Texture::get_texture(1); 

			mc.speed = 7.0f;

			mc.angle = *(float*)(&data);

			mc.velocity.x = cosf(mc.angle);
			mc.velocity.y = sinf(mc.angle);

			pc.damage = 4;

			health.entity_type = ENTITY_TYPE_BULLET;

			health.max_health = 1.0f;

			health.health = health.max_health;

			Game::coordinator->add_component<PositionComponent>(e, positionc);
			Game::coordinator->add_component<ColliderComponent>(e, cc);

			Game::coordinator->add_component<SizeComponent>(e, sc);
			Game::coordinator->add_component<RenderComponent>(e, rc);

			Game::coordinator->add_component<MovementComponent>(e, mc);
			Game::coordinator->add_component<ProjectileComponent>(e, pc);

			Game::coordinator->add_component<HealthComponent>(e, health);

		});
	func_pointers.push_back([](const Entity& e, const float& x, const float& y, uint32_t data) // textbox 
		{
			auto positionc = PositionComponent();

			auto sc = SizeComponent();
			auto rc = RenderComponent();

			auto mc = MovementComponent();

#ifdef ECS_DEBUG
			positionc.entity = e;

			sc.entity = e;
			rc.entity = e;

			mc.entity = e;
#endif

			positionc.pos.x = x;
			positionc.pos.y = y;

			sc.size.x = 30.0f;
			sc.size.y = 30.0f;

			rc.src_rect = { 0, 0, 400, 503 }; // TODO: determine texture

			rc.texture = Texture::get_texture(TEXTURE_MOLE); // TODO: determine texture

			mc.speed = 1.5f;


			Game::coordinator->add_component<PositionComponent>(e, positionc);

			Game::coordinator->add_component<SizeComponent>(e, sc);
			Game::coordinator->add_component<RenderComponent>(e, rc);

			Game::coordinator->add_component<MovementComponent>(e, mc);


		});
}

void EntityCreator::create_entity(const size_t& type, const float& x, const float& y, uint32_t data)
{
	SDL_assert(Game::coordinator != nullptr);
	Entity e = Game::coordinator->create_entity();


	const auto& func = func_pointers[type];
	func(e, x, y, data);
}

unsigned int EntityCreator::create_player_id()
{
	static uint8_t id = -1;
	return ++id;
}