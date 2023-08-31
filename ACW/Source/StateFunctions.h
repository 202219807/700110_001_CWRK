#pragma once
#include "State.h"

class Agent;
class State;

//------------------------------------------------------------------------
//	In this state the agent will be respawned at the team spawn location. 
//	
//	Once the agent is initialized, it changes it state to *MoveTowardsTower*
//------------------------------------------------------------------------
class MoveTowardsSpawnBase : public State
{
	MoveTowardsSpawnBase() {}
	MoveTowardsSpawnBase(const MoveTowardsSpawnBase&);
	MoveTowardsSpawnBase& operator=(const MoveTowardsSpawnBase&);

public:
	static MoveTowardsSpawnBase* Instance();

	virtual void Enter(Agent* agent);
	virtual void Execute(Agent* agent);
	virtual void Exit(Agent* agent);
};

//------------------------------------------------------------------------
//	In this state the agent will walk towards a tower. 
// 
//	If the agent reaches tower, it'll change state to *EnterIntoTower*
//------------------------------------------------------------------------
class MoveTowardsTower : public State
{
	MoveTowardsTower() {}
	MoveTowardsTower(const MoveTowardsTower&);
	MoveTowardsTower& operator=(const MoveTowardsTower&);

public:
	static MoveTowardsTower* Instance();

	virtual void Enter(Agent* agent);
	virtual void Execute(Agent* agent);
	virtual void Exit(Agent* agent);
};

//------------------------------------------------------------------------
//	In this state, if the tower is neutral, agent enters *InfluenceTower* 
//	state. 
//	
//	If the tower is controlled by opponent team, the agent enters 
//	*CaptureTower* state.
//------------------------------------------------------------------------
class EnterIntoTower : public State
{
	EnterIntoTower() {}
	EnterIntoTower(const EnterIntoTower&);
	EnterIntoTower& operator=(const EnterIntoTower&);

public:
	static EnterIntoTower* Instance();

	virtual void Enter(Agent* agent);
	virtual void Execute(Agent* agent);
	virtual void Exit(Agent* agent);
};

//------------------------------------------------------------------------
//	In this state, if the agent influences the tower to score points.
//	
//	If the agent's health is reduced to zero, the agent enters 
//	*MoveToSpawnBase* state.
//------------------------------------------------------------------------
class InfluenceTower : public State
{
	InfluenceTower() {}
	InfluenceTower(const InfluenceTower&);
	InfluenceTower& operator=(const InfluenceTower&);

public:
	static InfluenceTower* Instance();

	virtual void Enter(Agent* agent);
	virtual void Execute(Agent* agent);
	virtual void Exit(Agent* agent);
};

//------------------------------------------------------------------------
//	In this state, the agent attacks the agent controlling the tower.
// 
//	If the agent is able to eliminate the opponent, it then changes its
//	state to *InfluenceTower*
// 
//	If the agent's health is reduced to zero while attacking, the agent enters 
//	*MoveToSpawnBase* state.
//------------------------------------------------------------------------
class CaptureTower : public State
{
	CaptureTower() {}
	CaptureTower(const CaptureTower&);
	CaptureTower& operator=(const CaptureTower&);

public:
	static CaptureTower* Instance();

	virtual void Enter(Agent* agent);
	virtual void Execute(Agent* agent);
	virtual void Exit(Agent* agent);
};
