#pragma once
#include "Agent.h"
class Agent;
class State
{
public:
	virtual ~State() {}

	//this will execute when the state is entered
	virtual void Enter(Agent*) = 0;
	
	//this is called by the agents’s update function each update step
	virtual void Execute(Agent*) = 0;
	
	//this will execute when the state is exited
	virtual void Exit(Agent*) = 0;
};

