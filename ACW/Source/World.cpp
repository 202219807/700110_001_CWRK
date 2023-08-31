#include "World.h"
#include "Config.h"
#include "Tower.h"
#include "Projectile.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

#include <iostream>
#include <fstream>
#include <vector>
#include "imgui/imgui.h"

using namespace std;
using namespace glm;

World::Team World::m_RedTeam;
World::Team World::m_BlueTeam;

// TO DO : Refactor helper functions to utilities
vector<int> loadConfig(const char* filename)
{
	vector<int> config;
	ifstream inputFile(filename);

	if (!inputFile) cerr << "Could not open the file!" << endl;
	else
	{
		// Push items into a vector
		int current_number = 0;
		while (inputFile >> current_number) {
			config.push_back(current_number);
		}

		// Close the file.
		inputFile.close();
	}
	return config;
}

// Agent Wall Detection
bool World::checkForCollisionAgentWall(Agent* a) {
	float RWlenght, LWlenght, FWlenght, BWlenght;
	RWlenght = (GRID_WIDTH - 2.5f) - a->GetPosition().x;
	LWlenght = a->GetPosition().x;
	FWlenght = (GRID_DEPTH - 2.5f) - a->GetPosition().z;
	BWlenght = a->GetPosition().z;
	if(RWlenght < a->GetBoundingRadius() || LWlenght < a->GetBoundingRadius() || FWlenght < a->GetBoundingRadius() || BWlenght < a->GetBoundingRadius())
	{
		a->SetVelocity(-(a->GetVelocity()));
	}
	return true;
}

//Agent Ball Detection 
bool World::checkForCollisionBallWall(Projectile* b) {
	float RWlenght, LWlenght, FWlenght, BWlenght;
	RWlenght = (GRID_WIDTH - 2.5f) - b->GetPosition().x;
	LWlenght = b->GetPosition().x;
	FWlenght = (GRID_DEPTH - 2.5f) - b->GetPosition().z;
	BWlenght = b->GetPosition().z;
	if (RWlenght < b->GetBoundingRadius() || LWlenght < b->GetBoundingRadius() || FWlenght < b->GetBoundingRadius() || BWlenght < b->GetBoundingRadius())
	{
		std::cout << "Ball touching the wall!" << std::endl;
		b->SetVelocity(-(b->GetVelocity()));
	}
	return true;
}

//Sphere Cylinder Detection 
bool World::sphereCylinderCollision(Projectile* b) {
	for (int i = 0; i < _NUM_TOWERS; i++) {
		glm::vec3 U = b->GetVelocity();

		//Vector from sphere center to cylinder center 
		glm::vec3 d = b->GetPosition() - m_pTowers[i]->GetPosition();
		//Project d onto cylinder axis 
		float dist = glm::dot(d, glm::vec3(0, 1, 0));
		//check if sphere is within cylinder height 
		float Height = m_pTowers[i]->GetBaseHeight() + m_pTowers[i]->GetLipHeight();

		glm::vec3 N = glm::normalize(d);
		glm::vec3 V = glm::reflect(b->GetVelocity(), N);

		if (dist < 0 || dist > Height) {
			return false;
		}
		if (glm::length(d) < b->GetBoundingRadius() + m_pTowers[i]->GetBaseRadius()) {
			std::cout << "Ball colliding with tower" << std::endl;
			b->SetVelocity(V);
			return true;
		}
	}
	return false;
}

//Sphere Sphere Detection 
void World::sphereSphereCollision(Projectile* b1, Projectile* b2) {
	float e = 1.0f;
	glm::vec3 d = b1->GetPosition() - b2->GetPosition();
	glm::vec3 N = glm::normalize(d);
	glm::vec3 VN1 = (((b1->GetMass() - (e * b2->GetMass())) * (glm::dot(b1->GetVelocity(), N)) * N) + ((b2->GetMass() + e * b2->GetMass()) * (glm::dot(b2->GetVelocity(), N)) * N)) / (b1->GetMass() + b2->GetMass());
	glm::vec3 VN2 = (((b1->GetMass() + (e * b1->GetMass())) * (glm::dot(b1->GetVelocity(), N)) * N) + ((b2->GetMass() - e * b1->GetMass()) * (glm::dot(b2->GetVelocity(), N)) * N)) / (b1->GetMass() + b2->GetMass());
	glm::vec3 V1 = b1->GetVelocity() - (glm::dot(b1->GetVelocity(), N) * N) + VN1;
	glm::vec3 V2 = b2->GetVelocity() - (glm::dot(b2->GetVelocity(), N) * N) + VN1;
	if (glm::length(d) < b2->GetBoundingRadius() + b1->GetBoundingRadius()) {					//Collision Detection
		b1->SetVelocity(V1);																	//Collision Response
		b2->SetVelocity(-V2);
	}
}

// Agent Sphere Detection 
void World::agentSphereCollision(Agent* a, Projectile* b) {
	glm::vec3 D = a->GetPosition() - b->GetPosition();
	float length = glm::length(D);
	if (length < b->GetBoundingRadius() + a->GetBoundingRadius()) {
		std::cout << "Agent has been hit!" << std::endl;
		float newHealth = a->GetHealth() - 10.0f;
		a->SetHealth(newHealth);
		if (newHealth <= 0.0f) {
			a->Respawn();
		}
	}
}


void InitializeMap(World* pWorld)
{
	vector<int> heightMap = loadConfig("map_height.txt");
	vector<int> colourMap = loadConfig("map_colour.txt");
	vector<int> obstacleMap = loadConfig("map_obstacle.txt");
	vector<int> walkableMap = loadConfig("map_walkable.txt");

	vector<Tile*> obstacles;

	int index = 0;
	for (int i = 0; i < pWorld->GRID_WIDTH; i++)
	{
		for (int j = 0; j < pWorld->GRID_DEPTH; j++)
		{
			// setup grid walls
			pWorld->_gridHeights[i][j] = heightMap[index];

			// color grid tiles
			if (colourMap[index] == 1) pWorld->_gridColours[i][j] = vec3(0.0f, 0.5f, 0.0f);
			else if (colourMap[index] == 2) pWorld->_gridColours[i][j] = vec3(0.1f, 0.8f, 0.2f);
			else pWorld->_gridColours[i][j] = vec3(1);

			// add obstacles
			if (obstacleMap[index] != 0)
			{
				Tile* obs = new Tile;
				obstacles.push_back(obs);
			}
			else obstacles.push_back(0);

			pWorld->_gridObstacles[i][j] = obstacleMap[index];


			// TO DO - Remove Tower area from walkable region
			// TO DO - Add Tile Type: Walkable (Safezone, Neutral, Team1 Adv., Team2 Adv., Spawn base)
			// if (walkableMap[index] == 1) pWorld->_gridColours[i][j] = vec3(0.0f, 0.0f, 1.0f);

			index++;
		}
	}

	// terrain for collision detection
	pWorld->GetTerrain().Scale(vec3(50.0f, 0.1f, 50.0f));
	pWorld->GetTerrain().Translate(vec3(0.49f, 0.0f, 0.49f));
	
	//pWorld->GetLeftWall().Scale(vec3(0.1f, 50.0f, 50.0f));
	//pWorld->GetLeftWall().Translate(vec3(10.0f, 5.0f, 25.0f));

	//pWorld->GetRightWall().Scale(vec3(0.1f, 50.0f, 50.0f));
	//pWorld->GetRightWall().Translate(vec3(0.1f, 49.0f, 49.0f));

	//pWorld->GetFrontWall().Scale(vec3(50.0f, 50.0f, 0.1f));
	//pWorld->GetFrontWall().Translate(vec3(49.0f, 49.0f, 0.0f));

	//pWorld->GetRearWall().Scale(vec3(50.0f, 50.0f, 0.1f));
	//pWorld->GetRearWall().Translate(vec3(0.1f, 0.1f, 0.0f));

};

void InitializeGameEntities(vector<Tower*> pTowers, vector<Projectile*> pAmmo, vector<Agent*> pAgents)
{
	// Build Towers
	vector<vec3> towerLocations = _LOC_TOWERS;

	// int start = 0, end = (pAmmo.size() / 4) - 1;
	for (int i = 0; i < _NUM_TOWERS; i++)
	{
		pTowers[i]->SetPosition(towerLocations[i]);
		
		// vector<Projectile*> a = { pAmmo.begin() + start, pAmmo.begin() + end };
		vector<Projectile*> a2 = { pAmmo[i] };
		pTowers[i]->SetAmmunation(a2, towerLocations[i]);
		// start += pAmmo.size() / 4; end += pAmmo.size() / 4;
	}

	// Spawn Agents
	// Read team base positions from config
	vector<vec3> spawnLocations = _LOC_SPAWN_BASES;

	World::m_RedTeam._base = spawnLocations[0];
	World::m_BlueTeam._base = spawnLocations[1];

	for (int i = 0; i < _NUM_OF_AGENTS_PER_TEAM; i++)
	{
		// Team assignment
		World::m_RedTeam._members.push_back(pAgents[i]);
		World::m_BlueTeam._members.push_back(pAgents[_NUM_OF_AGENTS_PER_TEAM + i]);

		pAgents[i]->SetTeamId(1);
		pAgents[_NUM_OF_AGENTS_PER_TEAM + i]->SetTeamId(2);

		spawnLocations[0].z = spawnLocations[0].z + 1.3f;
		pAgents[i]->SetPosition(spawnLocations[0]);

		spawnLocations[1].x = spawnLocations[1].x + 1.3f;
		pAgents[_NUM_OF_AGENTS_PER_TEAM + i]->SetPosition(spawnLocations[1]);
		
		// Team color
		glm::vec3& red = pAgents[i]->GetColour();
		red.x = 0.8f;
		red.y = 0.5f;
		red.z = 0.0f;

		glm::vec3& blue = pAgents[_NUM_OF_AGENTS_PER_TEAM + i]->GetColour();
		blue.x = 0.0f;
		blue.y = 0.05f;
		blue.z = 0.9f;
	}
};

vec2 GetGridIndexFromWorldCoordinates(vec3 vPos)
{
	// TO DO - Add Tile width and Grid measurements
	int x = vPos.x / 1;
	if (vPos.x <= 0 || vPos.x >= 0 && x * 1 == vPos.x)
		vPos.x = vPos.x - 1;

	int z = vPos.z / 1;
	if (vPos.z <= 0 || vPos.z >= 0 && z * 1 == vPos.z)
		vPos.z = vPos.z - 1;

	vec2 ij = vec2(floor(vPos.x), floor(vPos.z));

	return ij;
}

int GetLinearIndexFromGridIndex(vec2 vId)
{
	// TO DO - Return linear index from vId.x, vId.y
	int id = 0;

	return id;
}

// Class function definitions
World::World()
{
	InitializeMap(this);

	std::shared_ptr<Integrator> integrator = physicsMgr.getIntegrator();

	for (auto i = 0; i < 2 * _NUM_OF_AGENTS_PER_TEAM; i++)
	{
		Agent* a = new Agent(this);
		m_pAgents.push_back(a);
	}
	for (auto i = 0; i < _NUM_TOWERS; i++)
	{
		Tower* t = new Tower;
		m_pTowers.push_back(t);
	}
	for (auto i = 0; i < m_MaxAmmo; i++)
	{
		Projectile* p = new Projectile(integrator);
		p->SetMass(1.0f);
		p->SetBoundingRadius(0.5f);
		p->SetMaxSpeed(60.0f);
		p->SetMaxResilience(60.0f);
		m_pAmmo.push_back(p);
	}

	InitializeGameEntities(m_pTowers, m_pAmmo, m_pAgents);

	// Init test objects
	// vec3 p = m_TestTile.GetPosition();
	// p.y = 5.0f;
	// p.x = 10.50f;
	// p.z = 25.1f;

	// m_TestTile.Translate(p);

	// vec3 q = m_TestProjectile.GetPosition();
	// q.y = 3.3f;
	// q.x = 25.50f;
	// q.z = 9.0f;

	// m_TestProjectile.UpdateIntegrator(integrator);
	// m_TestProjectile.SetPosition(q);
	// m_TestProjectile.SetBoundingRadius(0.5f);
	// m_TestProjectile.SetMass(1.0f);

}

void World::Update(float pDeltaTime)
{
	// Update game entities
	std::shared_ptr<Integrator> integrator = physicsMgr.getIntegrator();

	for (auto& a : m_pAgents)
	{
		checkForCollisionAgentWall(a);
		a->Update(pDeltaTime);
		a->_deltaTime = pDeltaTime;
	}
	
	
	// Handle all collision with sphere
	for (Projectile* cb : m_pAmmo)
	{
		
		// Sphere to Wall collision 
		checkForCollisionBallWall(cb);
		sphereCylinderCollision(cb);
		
		// Sphere to Ground collision
		cb->HandleCollision(m_Ground, vec3(0.0f, 1.0f, 0.0f));

		// Sphere to Sphere collision
		for (auto t : m_pTowers)
		{
			vector<Projectile*> m_pTowerAmmo = t->GetAmmunation();
			for (auto p : m_pTowerAmmo)
			{
				if (length(p->GetVelocity()) > 0.0f && p != cb)
				{
					sphereSphereCollision(p, cb);
				}
			}
		}

		for (auto p : m_pAmmo)
		{
			if (cb != p && cb->GetPosition() != p->GetPosition()) 
			{
					sphereSphereCollision(p, cb);
			};
		}
		
		// Sphere to Agent collision
		for (auto a : m_pAgents)
		{
			agentSphereCollision(a, cb);
		}

		if (length(cb->GetVelocity()) > 0.0f)
		{
			cb->UpdateIntegrator(integrator);
			cb->Update(pDeltaTime);	
		}
	}

	for (auto& t : m_pTowers)
	{
		t->Update(pDeltaTime);
	}

	SetElapsedTime(pDeltaTime);
	
	// TO DO -
	// Update score
	// Update map - ex spawn bonus points
}

void World::Render(const IRenderHelpers& pHelper) const
{
	// Render entities in the world
	pHelper.Render(this);

	for (auto& i : m_pAgents)
		pHelper.Render(i);

	for (auto& i : m_pTowers)
		pHelper.Render(i);

	for (auto& i : m_pAmmo)
		pHelper.Render(i);

	pHelper.Render(&m_Ground);
	pHelper.Render(&m_LeftWall);
	pHelper.Render(&m_RightWall);
	pHelper.Render(&m_FrontWall);
	pHelper.Render(&m_RearWall);

	// Render test objects
}

void World::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

int World::GetGridHeight(int i, int j) const
{
	return _gridHeights[i][j];
}

vec3 World::GetGridColour(int i, int j) const
{
	return _gridColours[i][j];
}

int World::GetGridObstacle(int i, int j) const
{
	return _gridObstacles[i][j];
}

bool World::TimeUp() const
{
	return m_ElapsedTime < 0;
}

void World::SetElapsedTime(float fTime)
{
	m_ElapsedTime -= fTime;
}

float World::GetElapsedTime()
{
	return m_ElapsedTime;
}

void World::UpdateScore()
{
	// update score here.
}

Tile& World::GetTerrain()
{
	return m_Ground;
};

Tile& World::GetLeftWall()
{
	return m_LeftWall;
};

Tile& World::GetRightWall()
{
	return m_RightWall;
};

Tile& World::GetFrontWall()
{
	return m_FrontWall;
};

Tile& World::GetRearWall()
{
	return m_RearWall;
};