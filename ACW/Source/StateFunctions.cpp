#include "StateFunctions.h"

MoveTowardsSpawnBase* MoveTowardsSpawnBase::Instance()
{
	static MoveTowardsSpawnBase instance;
	return &instance;
}

void MoveTowardsSpawnBase::Enter(Agent* agent)
{
	std::cout << "Entering spawn base.." << std::endl;
}

void MoveTowardsSpawnBase::Execute(Agent* agent)
{
	agent->Respawn();
	agent->ChangeState(MoveTowardsTower::Instance());
}

void MoveTowardsSpawnBase::Exit(Agent* agent)
{
	std::cout << "Agent ready to leave spawnbase" << std::endl;
}

MoveTowardsTower* MoveTowardsTower::Instance()
{
	static MoveTowardsTower instance;
	return &instance;
}

void MoveTowardsTower::Enter(Agent* agent)
{
	agent->FindNearestTower();
}

void MoveTowardsTower::Execute(Agent* agent)
{
	agent->Move();
	if (agent->IsAtTower())
		agent->ChangeState(EnterIntoTower::Instance());
}

void MoveTowardsTower::Exit(Agent* agent)
{
	std::cout << "Agent stopped moving." << std::endl;
}

EnterIntoTower* EnterIntoTower::Instance()
{
	static EnterIntoTower instance;
	return &instance;
}

void EnterIntoTower::Enter(Agent* agent)
{
	std::cout << "Agent entering into tower." << std::endl;
}

void EnterIntoTower::Execute(Agent* agent)
{
	// get instance of tower and check status
	if (agent->IsOpponentTower())
	{
		std::cout << "Tower is captured by opponent" << std::endl;
		agent->ChangeState(CaptureTower::Instance());
	}
	else
	{
		std::cout << "Tower is neutral" << std::endl;
		agent->ChangeState(InfluenceTower::Instance());
	}
}

void EnterIntoTower::Exit(Agent* agent)
{
	std::cout << "Leaving tower gates.." << std::endl;
}

InfluenceTower* InfluenceTower::Instance()
{
	static InfluenceTower instance;
	return &instance;
}

void InfluenceTower::Enter(Agent* agent)
{
	std::cout << "Agent controlling tower" << std::endl;
}

void InfluenceTower::Execute(Agent* agent)
{
	agent->Influence();
	std::cout << "Increasing the influence" << std::endl;

	if (agent->GetHealth() < 0.0f)
	{
		agent->ChangeState(MoveTowardsSpawnBase::Instance());
		std::cout << "Agent was killed while influencing the tower" << std::endl;
	}
}

void InfluenceTower::Exit(Agent* agent)
{
	std::cout << "Leaving the tower control" << std::endl;
}

CaptureTower* CaptureTower::Instance()
{
	static CaptureTower instance;
	return &instance;
}

void CaptureTower::Enter(Agent* agent)
{
	std::cout << "Guarding agent location spotted" << std::endl;
}

void CaptureTower::Execute(Agent* agent)
{
	agent->Attack();
	std::cout << "Agent attacking the guarding agent" << std::endl;
	if (agent->GetTargetAgent()->GetHealth() < 0.0f);
	{
		std::cout << "Guarding agent got killed" << std::endl;
		agent->ChangeState(InfluenceTower::Instance());
	}

	if (agent->GetHealth() < 0.0f)
	{
		std::cout << "Attacking agent got killed" << std::endl;
		agent->ChangeState(MoveTowardsSpawnBase::Instance());
	}
}

void CaptureTower::Exit(Agent* agent)
{
	std::cout << "Ending combat at tower gate" << std::endl;
}
