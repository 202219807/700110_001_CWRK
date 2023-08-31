#pragma once

#include "GameEntity.h"
#include "Projectile.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

using namespace std;
using namespace glm;

class Projectile;
class IRenderHelpers;
class IGuiHelpers;

class Tower : public GameEntity
{
private:
	// Tower influence
	int m_OwnerId = 0;
	float m_InfluenceLvl = 0.0f;

	// Tower Defense Config
	vector<Projectile*> m_Ammo;
	bool m_RequestAmmo = false;
	float m_ShootInterval = 0.0f;
	float m_ShootSpeed = 0.0f;
	
	// Tower Geometry
	float m_BaseRadius = 0.0f;
	float m_LipRadius = 0.0f;
	float m_BaseHeight = 0.0f;
	float m_LipHeight = 0.0f;

	
public:
	Tower();
	Tower(const Tower&) = delete;
	Tower& operator=(const Tower&) = delete;
	~Tower() {};

	float GetBaseRadius() const;
	void SetBaseRadius(float);

	float GetLipRadius() const;
	void SetLipRadius(float);

	float GetBaseHeight() const;
	void SetBaseHeight(float);

	float GetLipHeight() const;
	void SetLipHeight(float);

	float GetShootInterval() const;
	void SetShootInterval(float);

	float GetShootSpeed() const;
	void SetShootSpeed(float);

	void SetAmmunation(vector<Projectile*>, vec3);
	vector<Projectile*> GetAmmunation() const;
	
	bool IsNeutral();

	void SetOwnerId(int id);
	int GetOwnerId();

	void SetInfluenceLevel(float);
	float GetInfluenceLevel();
	
	void Update(float);
	void Render(const IRenderHelpers&) const override;
	void RenderGui(const IGuiHelpers&) override;
};
