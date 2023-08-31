#include "Agent.h"
#include "World.h"
#include "StateFunctions.h"

Agent::Agent() :
	m_Health(0.0f),
	m_Team(0),
	m_pWorld(nullptr),
	m_pSteering(nullptr),
	m_pCurrentState(nullptr),
	m_pTargetTower(nullptr),
	m_pTargetAgent(nullptr),
	Physics()
{
	std::cout << "Initializing agent" << std::endl;
	m_pSteering = nullptr;
	m_fTimeElapsed = 0.0f;
	SetMaxSpeed(0.0f);
	SetMass(50.0f);
}

Agent::Agent(World* pWorld) :
	m_Health(100.0f),
	m_Team(0),
	m_pWorld(pWorld),
	m_pSteering(),
	m_pCurrentState(MoveTowardsSpawnBase::Instance()),
	m_pTargetTower(nullptr),
	m_pTargetAgent(nullptr),
	Physics()
{
	std::cout << "Initializing agent with steering behaviour" << std::endl;
	m_pSteering = new SteeringBehaviours(this);
	m_fTimeElapsed = 0.0f;
	SetMaxSpeed(20.0f);
	SetMass(50.0f);
	SetBoundingRadius(1.0f);
}

Agent::~Agent()
{
	delete m_pSteering;
};

void Agent::Update(float dt)
{
	// update agent stats here as per gameplay
	if (m_pCurrentState)
		m_pCurrentState->Execute(this);
}

void Agent::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Agent::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

void Agent::ChangeState(State* pNewState)
{
	//make sure both states are valid before attempting to
	//call their methods
	assert(m_pCurrentState && pNewState);

	//call the exit method of the existing state
	m_pCurrentState->Exit(this);

	//change state to the new state
	m_pCurrentState = pNewState;

	//call the entry method of the new state
	m_pCurrentState->Enter(this);
}

State* const Agent::AgentState() const
{
	return m_pCurrentState;
}

Tower* Agent::GetTargetTower()
{
	return m_pTargetTower;
}

void Agent::SetTargetTower(Tower* pTower)
{
	m_pTargetTower = pTower;
}

Agent* Agent::GetTargetAgent()
{
	return m_pTargetAgent;
};

void Agent::SetTargetAgent(Agent* pAgent)
{
	m_pTargetAgent = pAgent;
}

bool Agent::IsAtTower()
{
	// compare linearid/gridid with target tower's id

	// m_pTargetTower->GetGridId();
	// vec3 worldPos = GetPosition();
	// vec2 gridId = GetGridIndexFromWorldCoordinates(worldPos);
	// int linearId = GetLinearIndexFromGridIndex(gridId);

	std::cout << "Checking if agent reached the tower." << std::endl;
	auto pos = GetPosition();
	if (m_pTargetTower && m_pTargetTower->GetPosition() == GetPosition())
	{
		pos.x += 1;
		pos.y = 5.0f;
		pos.z += 1;
		SetPosition(pos);
		return true;
	}

	return false;
}

bool Agent::IsOpponentTower()
{
	if (!m_pTargetTower->IsNeutral() && GetTeamId() != m_pTargetTower->GetOwnerId())
		return true;
	return false;
}

void Agent::Respawn()
{
	// Reset health and position
	int teamId = GetTeamId() - 1;
	vec3 spawnLoc = m_pWorld->GetSpawnLocations(teamId);
	int agentId = ID();

	if (teamId == 1)
	{
		spawnLoc.z += 3.0f;
		spawnLoc.x -= 1.3f * (static_cast<float>(agentId) - 330.0f);
	}
	else spawnLoc.z += 1.3f*(static_cast<float>(agentId) - 326.0f);
	
	SetPosition(spawnLoc);
	SetHealth(100.0f);
}

void Agent::FindNearestTower()
{
	std::cout << "Finding path towards tower" << std::endl;
	vector<Tower*> towers = GameWorld()->GetTowers();
	Tower* target = towers[0];
	
	// TO DO - Review again
	/*auto agentPos = GetPosition();

	float nearestDistance = 9999999.99f;
	for (auto& t : towers)
	{
		float distance = length(agentPos - t->GetPosition());
		if (distance < nearestDistance)
		{
			if (t->GetOwnerId() != GetTeamId())
			{
				for (auto i : m_pWorld->GetPlayers(GetTeamId() - 1))
				{
					if (i->GetTargetTower() != t)
					{
						nearestDistance = distance;
						target = t;

					}
				}
			}
		}
	}*/

	int teamId = GetTeamId() - 1;
	int agentId = ID();

	if (teamId == 1) target = towers[agentId - 330];
	else target = towers[agentId - 326];


	std::cout << "Nearest tower is at: ["
		<< target->GetPosition().x << " "
		<< target->GetPosition().y << " "
		<< target->GetPosition().z << "]"
		<< std::endl;
	SetTargetTower(target);
}

void Agent::Attack()
{
	// agent enemy agent
}

void Agent::Influence()
{
	// influence tower and update score card
}

void Agent::Move()
{
	std::cout << "Moving towards tower" << std::endl;
	vec3 SteeringForce;
	SteeringForce = m_pSteering->Calculate();

	vec3 acceleration = SteeringForce / this->GetMass();

	float prevSpeed = this->GetSpeed();
	vec3 vel = this->GetVelocity();
	vec3 pos = this->GetPosition();

	if (prevSpeed < this->GetMaxSpeed())
		vel += acceleration * _deltaTime;

	pos += vel * _deltaTime;

	this->SetVelocity(vel);
	this->SetPosition(pos);
}

float Agent::GetHealth()
{
	return m_Health;
}

void Agent::SetHealth(float fHealth)
{
	m_Health = fHealth;
}

int Agent::GetTeamId()
{
	return m_Team;
}

void Agent::SetTeamId(int id)
{
	m_Team = id;
}