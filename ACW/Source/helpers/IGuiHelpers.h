#pragma once

class GameEntity;
class World;
class Agent;
class Tower;
class Projectile;
class Camera;

class IGuiHelpers
{
public:
	virtual void RenderGui(GameEntity*) const = 0;
	virtual void RenderGui(World*) const = 0;
	virtual void RenderGui(Agent*) const = 0;
	virtual void RenderGui(Tower*) const = 0;
	virtual void RenderGui(Projectile*) const = 0;
	virtual void RenderGui(Camera*) const = 0;
};