#include "Physics.h"

#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

Physics::Physics() :
	GameEntity(),
	m_Mass(0.0f),
	m_BoundingRadius(0.0f),
	m_Speed(0.0f),
	m_MaxSpeed(0.0f),
	m_MaxResilience(0.0f),
	m_PreviousPosition(vec3(0)),
	m_Velocity(vec3(0)),
	m_Force(vec3(0)),
	m_bIsInFlight(false),
	m_Integrator(nullptr)
{
}

Physics::Physics(std::shared_ptr<Integrator> integrator) :
	GameEntity(),
	m_Mass(1.0f),
	m_BoundingRadius(1.0f),
	m_Speed(0.0f),
	m_MaxSpeed(10.0f),
	m_MaxResilience(0.0f),
	m_PreviousPosition(vec3(0)),
	m_Velocity(vec3(0)),
	m_Force(vec3(0)),
	m_bIsInFlight(false),
	m_Integrator(integrator)
{
}

void Physics::Simulate(float dt)
{
	// Perform numerical integration
	vec3 s = GetPosition();
	vec3 v = m_Velocity;
	float m = m_Mass;

	vec3 fgravity(0.0f, _GRAVITATIONAL_ACCELERATION, 0.0f);
	vec3 fmuzzle(0.3f, 0.8f, 0.0f);

	std::vector<vec3> resultSet;
	if (m_Speed < m_MaxSpeed)
	{
		resultSet = m_Integrator->integrate(dt, s, v, m, fgravity, fmuzzle);

		SetPosition(resultSet[0]);
		SetVelocity(resultSet[1]);
	}

	if (resultSet.size() > 0)
		SetSpeed(length(resultSet[1]));
}

void Physics::UpdateIntegrator(std::shared_ptr<Integrator> pIntegrator)
{
	m_Integrator = pIntegrator;
};

float Physics::GetMass() const
{
	return m_Mass;
}

void Physics::SetMass(float fMass)
{
	m_Mass = fMass;
}

float Physics::GetBoundingRadius() const
{
	return m_BoundingRadius;
}

void Physics::SetBoundingRadius(float fRadius)
{
	m_BoundingRadius = fRadius;
}

float Physics::GetSpeed() const
{
	return length(m_Velocity);
}

void Physics::SetSpeed(float fSpeed)
{
	m_Speed = fSpeed;
}

float Physics::GetMaxSpeed() const
{
	return m_MaxSpeed;
}

void Physics::SetMaxSpeed(float fMaxSpeed)
{
	m_MaxSpeed = fMaxSpeed;
}

float Physics::GetMaxResilience() const
{
	return m_MaxResilience;
}

void Physics::SetMaxResilience(float fMaxResilience)
{
	m_MaxResilience = fMaxResilience;
}

vec3 Physics::GetPrevPos()
{
	return m_PreviousPosition;
}

void Physics::SetPrevPos(vec3 vPrevPosition)
{
	m_PreviousPosition = vPrevPosition;
}

vec3 Physics::GetVelocity() const
{
	return m_Velocity;
}

void Physics::SetVelocity(vec3 vVelocity)
{
	if (length(vVelocity) < m_MaxSpeed)
		m_Velocity = vVelocity;
}

vec3 Physics::GetForce() const
{
	return m_Force;
}

void Physics::SetForce(vec3 vForce)
{
	m_Force = vForce;
}
bool Physics::IsInFlight() const
{
	return m_bIsInFlight;
}

void Physics::SetIsInFlight(bool bFlag)
{
	m_bIsInFlight = bFlag;
}