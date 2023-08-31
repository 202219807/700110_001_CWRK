#include "GuiHelpers.h"

/*
Dear ImGui is a bloat-free graphical user interface library for C++.
https://github.com/ocornut/imgui
*/
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../GameEntity.h"
#include "../Camera.h"
#include "../World.h"
#include "../Tower.h"
#include "../Projectile.h"
#include "../Agent.h"

void ImGuiHelpers::RenderGui(string pLabel, float pValue) const
{
	pLabel = pLabel + "[" + std::to_string(pValue) + "]";
	ImGui::Text(pLabel.c_str());
}

void ImGuiHelpers::RenderGui(string pLabel, glm::vec2 pVec2) const
{
	pLabel = pLabel + "[" + std::to_string(pVec2.x) + "," + std::to_string(pVec2.y) + "]";
	ImGui::Text(pLabel.c_str());
}

void ImGuiHelpers::RenderGui(string pLabel, glm::vec2 * pVec2) const
{
	pLabel = pLabel + "[" + std::to_string(pVec2->x) + "," + std::to_string(pVec2->y) + "]";
	ImGui::Text(pLabel.c_str());
}

void ImGuiHelpers::RenderGui(string pLabel, glm::vec3 pVec3) const
{
	pLabel = pLabel + "[" + std::to_string(pVec3.x) + "," + std::to_string(pVec3.y) + "," + std::to_string(pVec3.z) + "]";
	ImGui::Text(pLabel.c_str());
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void ImGuiHelpers::RenderGuiAsColour(string pLabel, glm::vec3* pColour) const
{
	ImGui::ColorEdit3(pLabel.c_str(), &(*pColour)[0]);
	ImGui::SameLine(); 
	HelpMarker(
		"Click on the color square to open a color picker.\n"
		"Click and hold to use drag and drop.\n"
		"Right-click on the color square to show options.\n"
		"CTRL+click on individual component to input value.\n");
}

void ImGuiHelpers::RenderGui(string pLabel, glm::vec3* pVec3) const
{
	ImGui::InputFloat3(pLabel.c_str(), &(*pVec3)[0]);
}

void ImGuiHelpers::RenderGui(GameEntity* pEntity) const
{
	std::string label = "Entity : " + std::to_string(pEntity->ID());
	ImGui::Text(label.c_str());
}

void ImGuiHelpers::RenderGui(World* pWorld) const
{
	std::string label1 = "World Entity ID: " + std::to_string(pWorld->ID());
	ImGui::Text(label1.c_str());
	std::string label2 = "Game Time Remaining: " + std::to_string(pWorld->GetElapsedTime());
	ImGui::Text(label2.c_str());
}

void ImGuiHelpers::RenderGui(Tower* pTower) const
{
	std::string label = "Tower : " + std::to_string(pTower->ID());
	ImGui::Text(label.c_str());

	ImGui::Text("Position :");
	ImGui::SameLine();
	vec3 p = pTower->GetPosition();
	string attrib = "[" + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "]";
	ImGui::Text(attrib.c_str());

	//ImGui::Text("Ammunation Counter :");
	//ImGui::SameLine();
	//ImGui::Text("%d", pTower->GetAmmunation().size());

	ImGui::Text("Ownership :");
	ImGui::SameLine();
	auto ownerId = pTower->GetOwnerId();
	string owner = ownerId == 1 ? "Red Team" : (ownerId == 2 ? "Blue Team" : "Neutral");
	ImGui::Text("%s", owner);

	ImGui::Text("Influence Level :");
	ImGui::SameLine();
	ImGui::Text("%d", pTower->GetInfluenceLevel());

}

void ImGuiHelpers::RenderGui(Agent* pAgent) const
{
	ImGui::Text("Agent ID :");
	ImGui::SameLine();
	ImGui::Text("%d", pAgent->ID());

	ImGui::Text("Agent Health : ");
	ImGui::SameLine();
	float h = pAgent->GetHealth();
	ImGui::Text("%.2f", h);

	ImGui::Text("Agent Position : ");
	ImGui::SetNextItemWidth(250);
	vec3 p = pAgent->GetPosition();
	string attrib = "[" + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "]";
	ImGui::Text(attrib.c_str());

	
	auto tTower = pAgent->GetTargetTower();
	if (tTower)
	{
		ImGui::Text("Moving Towards Tower Position :");
		ImGui::SameLine();
		ImGui::Text("%d", tTower->GetPosition());
	}
	
	auto tAgent = pAgent->GetTargetAgent();
	if (tAgent)
	{
		ImGui::Text("Targeting Opponent ID :");
		ImGui::SameLine();
		ImGui::Text("%d", tAgent->ID());
	}

	ImGui::Text("----------------------------");
}


void ImGuiHelpers::RenderGui(Projectile* pProjectile) const
{
	std::string label = "Cannon Ball : " + std::to_string(pProjectile->ID());
	ImGui::Text(label.c_str());

	RenderGui("Position : ", pProjectile->GetPosition());
	RenderGui("Bounding Radius : ", pProjectile->GetBoundingRadius());
	RenderGui("Mass : ", pProjectile->GetMass());
	RenderGui("Velocity : ", pProjectile->GetVelocity());
}

void ImGuiHelpers::RenderGui(Camera* pCamera) const
{
	std::string label = "Camera ID: " + std::to_string(pCamera->ID());
	ImGui::Text(label.c_str());

	ImGui::Text("Set Camera Coordinates:");
	ImGui::SetNextItemWidth(250);
	RenderGui("", &pCamera->Position);
	ImGui::NewLine();
}