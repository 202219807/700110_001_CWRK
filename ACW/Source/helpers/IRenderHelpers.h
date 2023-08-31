#pragma once
#include <memory>
class GameEntity;
class Camera;
class World;
class Agent;
class Tower;
class CannonBall;
class Projectile;
class Tile;

class IRenderHelpers
{
private:

public:
	virtual ~IRenderHelpers() {}
	
	virtual void Init() = 0;
	virtual void SetSize(unsigned int pWidth, unsigned int pHeight) = 0;
	
	virtual void Render(const GameEntity*) const = 0;
	virtual void Render(const World* pWorld) const = 0;
	virtual void Render(const Agent* pAgent) const = 0;
	virtual void Render(const Tower* pTower) const = 0;
	virtual void Render(const Projectile* pProjectile) const = 0;
	virtual void Render(const std::unique_ptr<Projectile> pProjectile) const = 0;
	virtual void Render(const Tile* pTile) const = 0;
	virtual void Render(const Camera*) const = 0;
	// virtual void Render(const std::vector<Agent*> pTeam) const = 0;
};
