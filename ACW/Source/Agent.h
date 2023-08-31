#pragma once

#include "Physics.h"
#include "SteeringBehaviours.h"
#include "State.h"
#include "Tower.h"

#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

class World;
class SteeringBehavior;
class State;
class Tower;

class Agent : public Physics
{
	World* m_pWorld;
	State* m_pCurrentState;
	SteeringBehaviours* m_pSteering;
	Tower* m_pTargetTower;
	Agent* m_pTargetAgent;
	float m_Health;
	int m_Team;

	// Steering config
	
	// keeps a track of the most recent update time.
	float m_fTimeElapsed;

public:
	Agent();
	Agent(World*);
	Agent(const Agent&) = delete;
	Agent& operator=(const Agent&) = delete;
	~Agent();

	float _deltaTime = 0.0f;

	World* const GameWorld() const { return m_pWorld; }
	SteeringBehaviours* const Steering() const { return m_pSteering; }
	State* const AgentState() const;
	
	void ChangeState(State*);
	void Update(float);
	void Render(const IRenderHelpers&) const override;
	void RenderGui(const IGuiHelpers&) override;
	
	void Attack();
	void Influence();
	void Move(); 
	void Respawn();
	void FindNearestTower();

	bool IsAtTower();
	bool IsOpponentTower();

	void SetHealth(float);
	float GetHealth();

	void SetTeamId(int);
	int GetTeamId();

	void SetTargetTower(Tower*);
	Tower* GetTargetTower();

	void SetTargetAgent(Agent*);
	Agent* GetTargetAgent(); 
};

