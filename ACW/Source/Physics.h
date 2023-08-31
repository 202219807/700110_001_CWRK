#pragma once
#include "GameEntity.h"
#include "Integrator.h"
#include "Config.h"
#include <memory>

using namespace glm;

class IRenderHelpers;
class IGuiHelpers;

class Physics : public GameEntity
{
	std::shared_ptr<Integrator> m_Integrator;
	float m_Mass;
	float m_BoundingRadius;
	float m_Speed;
	float m_MaxSpeed;
	float m_MaxResilience;

	// Inherit "m_Position" value 
	// from GameEntity.h
	vec3 m_PreviousPosition;
	vec3 m_Velocity;
	vec3 m_Force;
	bool m_bIsInFlight;

public:
	Physics();
	Physics(std::shared_ptr<Integrator> integrator);
	Physics(const Physics&) = delete;
	Physics& operator=(const Physics&) = delete;
	~Physics() = default;

	void UpdateIntegrator(std::shared_ptr<Integrator> pIntegrator);
	void Simulate(float);
	virtual void Render(const IRenderHelpers&) const = 0;
	virtual void RenderGui(const IGuiHelpers&) = 0;

	// Getters-Setters
	float GetMass() const;
	void SetMass(float);

	float GetBoundingRadius() const;
	void SetBoundingRadius(float);

	float GetSpeed() const;
	void SetSpeed(float);

	float GetMaxSpeed() const;
	void SetMaxSpeed(float);

	float GetMaxResilience() const;
	void SetMaxResilience(float);

	vec3 GetPrevPos();
	void SetPrevPos(vec3);

	vec3 GetVelocity() const;
	void SetVelocity(vec3);

	vec3 GetForce() const;
	void SetForce(vec3);

	bool IsInFlight() const;
	void SetIsInFlight(bool);
};

class PhysicsManager {
public:
	static PhysicsManager& getInstance() 
	{
		static PhysicsManager* instance = new PhysicsManager();
		return *instance;
	}
	void setIntegrator(std::shared_ptr<Integrator> pIntegrator)
	{
		_integrator = pIntegrator;
	}
	std::shared_ptr<Integrator> getIntegrator() 
	{
		return _integrator;
	}

private:
	std::shared_ptr<Integrator> _integrator;
	PhysicsManager() 
	{
		_integrator = std::make_shared<EulerIntegrator>();
	}
	PhysicsManager(const PhysicsManager&) = delete;
	PhysicsManager& operator=(const PhysicsManager&) = delete;
};
