#pragma once

#include "IGuiHelpers.h"
#include "../glm/glm.hpp"
#include <string>

using std::string;

class ImGuiHelpers : public IGuiHelpers
{
	void RenderGui(string pLabel, float) const;
	void RenderGui(string pLabel, glm::vec2) const;
	void RenderGui(string pLabel, glm::vec2*) const;
	void RenderGui(string pLabel, glm::vec3) const;
	void RenderGui(string pLabel, glm::vec3*) const;
	void RenderGuiAsColour(string pLabel, glm::vec3*) const;
	
	void RenderGui(GameEntity*) const;
	void RenderGui(World*) const;
	void RenderGui(Agent*) const;
	void RenderGui(Tower*) const;
	void RenderGui(Projectile*) const;
	void RenderGui(Camera*) const;
};