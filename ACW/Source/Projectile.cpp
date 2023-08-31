#include "Projectile.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

Projectile::Projectile() : Physics()
{
}

Projectile::Projectile(std::shared_ptr<Integrator> pIntegrator): Physics(pIntegrator)
{
}

Projectile::Projectile(const Projectile& pProjectile): Physics(nullptr)
{
}

Projectile::~Projectile()
{
}

void Projectile::Update(float pDeltaTime)
{
	Simulate(pDeltaTime);
}

void Projectile::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Projectile::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

// Collision Detection and Resolution
// 
// Sphere to Sphere Collision
void Projectile::HandleCollision(Projectile* other)
{
	vec3 d = this->GetPosition() - other->GetPosition();
	if (length(d) < other->GetBoundingRadius() + this->GetBoundingRadius())
	{ 
		vec3 n = normalize(d);

		vec3 vL1 = ((this->GetMass() - _RESTITUTION * other->GetMass()) * (dot(this->GetVelocity(), n) * n) + (this->GetMass() + _RESTITUTION * other->GetMass()) * (dot(other->GetVelocity(), n)) * n) / (this->GetMass() + other->GetMass());
		vec3 vL2 = ((this->GetMass() + _RESTITUTION * other->GetMass()) * (dot(this->GetVelocity(), n)) * n) + (other->GetMass() - _RESTITUTION * this->GetMass()) * (dot(other->GetVelocity(), n)) * n / (this->GetMass() + other->GetMass());

		vec3 v1 = this->GetVelocity() - dot(this->GetVelocity(), n) * n + vL1;
		vec3 v2 = other->GetVelocity() - dot(other->GetVelocity(), n) * n + vL2;

		this->SetVelocity(v1);
		other->SetVelocity(-v2);
	}
}

// Sphere to Cylinder Collision
void Projectile::HandleCollision(Tower* other)
{
	vec3 u = GetVelocity();
	vec3 d = GetPosition() - other->GetPosition();
	float projection = dot(d, vec3(0.0f, 1.0f, 0.0f));
	float height = other->GetBaseHeight() + other->GetLipHeight();

	vec3 n = normalize(d);
	// vec3 v = u - (_RESTITUTION + 1.0f) * (dot(n, u) * n);
	vec3 v = reflect(GetVelocity(), n);

	if (projection < 0 || projection > height)
		return;

	if (length(d) < GetBoundingRadius() + other->GetBaseRadius()) {
		SetVelocity(v);
		return;
	}
}

// Sphere to Cuboid Collision
void Projectile::HandleCollision(Tile& other, vec3 n)
{
	vec3 u = GetVelocity(); // relative velocity to ground

	if (GetPosition().y + 0.5 <= 1.0f + GetBoundingRadius())
	{
		vec3 v = u - (1.0f + _RESTITUTION) * dot(n, u) * n;
		SetVelocity(vec3(v.x, v.y, v.z));
	}
}

// Sphere to Capsule 
void Projectile::HandleCollision(Agent* other)
{
	vec3 d = this->GetPosition() - other->GetPosition();
	if (length(d) < other->GetBoundingRadius() + this->GetBoundingRadius())
	{
		vec3 n = normalize(d);

		vec3 vL1 = ((this->GetMass() - _RESTITUTION * other->GetMass()) * (dot(this->GetVelocity(), n) * n) + (this->GetMass() + _RESTITUTION * other->GetMass()) * (dot(other->GetVelocity(), n)) * n) / (this->GetMass() + other->GetMass());
		vec3 vL2 = ((this->GetMass() + _RESTITUTION * other->GetMass()) * (dot(this->GetVelocity(), n)) * n) + (other->GetMass() - _RESTITUTION * this->GetMass()) * (dot(other->GetVelocity(), n)) * n / (this->GetMass() + other->GetMass());

		vec3 v1 = this->GetVelocity() - dot(this->GetVelocity(), n) * n + vL1;
		vec3 v2 = other->GetVelocity() - dot(other->GetVelocity(), n) * n + vL2;

		this->SetVelocity(v1);
		other->SetVelocity(-v2);
	}
}