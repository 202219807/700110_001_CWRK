#pragma once
#include "GameEntity.h"
#include "Config.h"
#include "Projectile.h"

using namespace std;
using namespace glm;

class Projectile;

class Tile : public GameEntity
{
public:
	Tile(): GameEntity() { };
	~Tile() { };

	void Update(float) { };
	void Render(const IRenderHelpers&) const override { };
	void RenderGui(const IGuiHelpers&) override { };
};