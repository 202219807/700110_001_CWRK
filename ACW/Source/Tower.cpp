#include "Tower.h"
#include "Projectile.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"
#include <iostream>
#include <vector>

Tower::Tower() : GameEntity()
{
	glm::vec3& colour = GetColour();
	colour.x = 0.7f;
	colour.y = 0.7f;
	colour.z = 0.7f;

	SetBaseHeight(2.75f);
	SetLipHeight(0.5f);
	SetBaseRadius(1.5f);
	SetLipRadius(1.8f);
	SetShootInterval(1.0f);
	SetShootSpeed(50.0f);

}

/*
	Shooting Algo

	Check if Ammo ?
	N: Turn on "RequestAmmo" 
	Y:
		Get Ball Reference
		Find Agent to Shoot
		Calculate Desired Velocity/Force
		
		Send Ball Reference back to World to Update free balls
		Update Ammunation Counter
	
	*/
void ShootProjectile(Projectile* pCannonBall, Tower* pTower, vec3 pDirection)
{
	cout << "Shooting projectile" << pCannonBall << endl;
	pCannonBall->SetIsInFlight(true);
	pCannonBall->SetVelocity(pDirection * pTower->GetShootSpeed());		
}

void Tower::Update(float pDeltaTime)
{
	float t = GetShootInterval();
	t -= pDeltaTime;
	
	if (t > 0) SetShootInterval(t);
	else
	{
		if (m_Ammo.size() <= 0) m_RequestAmmo = true;
		else
		{
			Projectile* p = m_Ammo.back();
			m_Ammo.pop_back();

			vec3 aPos = GetPosition();
			aPos.y = GetBaseHeight() + GetLipHeight();
			
			// TO DO - replace target with agent position to shoot
			float HI = 30.0f; float LO = 5.0f;
			float x = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
			float z = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
			vec3 bPos = vec3(30.0f, 4.0f, 35.f);
			
			vec3 pDirection = normalize(bPos - aPos);

			bool f = p->IsInFlight();
			// cout << "TOWER:: Checking if projectile " << p << " is in flight " << f << endl;
			if (!f) {
				// cout << "False, hencing moving in to shoot.." << endl;
				ShootProjectile(p, this, pDirection);
				SetShootInterval(5.0f);
			}
		}
	}
}

void Tower::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
};

void Tower::RenderGui(const IGuiHelpers& pGuiHelper)
{
	pGuiHelper.RenderGui(this);
}

float Tower::GetShootInterval() const
{
	return m_ShootInterval;
}

void Tower::SetShootInterval(float fTime)
{
	m_ShootInterval = fTime;
}

float Tower::GetShootSpeed() const
{
	return m_ShootSpeed;
}

void Tower::SetShootSpeed(float fTime)
{
	m_ShootSpeed = fTime;
}

void Tower::SetAmmunation(vector<Projectile*> lAmmo, vec3 vPos)
{
	vPos.y = 3.8f;
	for (auto i : lAmmo)
		i->SetPosition(vPos);

	m_Ammo.clear();
	m_Ammo = lAmmo;
}

vector<Projectile*> Tower::GetAmmunation() const
{
	return m_Ammo;
}

float Tower::GetBaseRadius() const
{
	return m_BaseRadius;
}

void Tower::SetBaseRadius(float fRadius)
{
	m_BaseRadius = fRadius;
}

float Tower::GetLipRadius() const
{
	return m_LipRadius;
}

void Tower::SetLipRadius(float fRadius)
{
	m_LipRadius = fRadius;
}

float Tower::GetBaseHeight() const
{
	return m_BaseHeight;
}

void Tower::SetBaseHeight(float fHeight)
{
	m_BaseHeight = fHeight;
}

float Tower::GetLipHeight() const
{
	return m_LipHeight;
}

void Tower::SetLipHeight(float fHeight)
{
	m_LipHeight = fHeight;
}

bool Tower::IsNeutral()
{
	if (m_OwnerId == 0) return true;
	return false;
}

void Tower::SetOwnerId(int id)
{
	m_OwnerId = id;
}

int Tower::GetOwnerId()
{
	return m_OwnerId;
}

void Tower::SetInfluenceLevel(float fLevel)
{
	m_InfluenceLvl = fLevel;
}

float Tower::GetInfluenceLevel()
{
	return m_InfluenceLvl;
}
