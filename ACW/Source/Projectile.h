#pragma once
#include "Physics.h"
#include "Tile.h"
#include "Tower.h"
#include "Agent.h"
#include "glm/glm.hpp"

class Tower;
class Agent;
class Tile;

class Projectile : public Physics
{

public:
	Projectile();
	Projectile(std::shared_ptr<Integrator>);
	Projectile(const Projectile&);
	Projectile& operator=(const Projectile&) = delete;

	~Projectile();

	void Update(float);
	void Render(const IRenderHelpers&) const override;
	void RenderGui(const IGuiHelpers&) override;
	
	// Collision
	void HandleCollision(Projectile*);
	void HandleCollision(Tower*);
	void HandleCollision(Tile&, vec3);
	void HandleCollision(Agent*);
};

