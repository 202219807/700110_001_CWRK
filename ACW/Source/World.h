#pragma once
#include "GameEntity.h"
#include "Agent.h"
#include "Projectile.h"
#include "Tower.h"
#include "Tile.h"

// TO DO - PowerUps
// TO DO - Score
class Tower;
class Projectile;

using namespace glm;

class World : public GameEntity
{
public:
	const static int GRID_WIDTH = 50;
	const static int GRID_DEPTH = 50;
	int _gridHeights[GRID_WIDTH][GRID_DEPTH];
	int _gridObstacles[GRID_WIDTH][GRID_DEPTH];
	vec3 _gridColours[GRID_WIDTH][GRID_DEPTH];
	struct Team 
	{
		float _score;
		vector<Agent*> _members;
		vec3 _base;

		Team() : _score(0.0f), _members({}), _base(vec3(0)) {};
	};
	static Team m_RedTeam;
	static Team m_BlueTeam;

private:
	PhysicsManager& physicsMgr = PhysicsManager::getInstance();
	Tile m_Ground;

	Tile m_LeftWall;
	Tile m_RightWall;
	Tile m_FrontWall;
	Tile m_RearWall;
	
	vector<Tile*> m_Obstacles;
	vector<Agent*> m_pAgents;
	vector<Tower*> m_pTowers;
	vector<Projectile*> m_pAmmo;
	const int m_MaxAmmo = 4;
	const float m_MaxGameTime = 60.0f;
	float m_ElapsedTime = m_MaxGameTime;

	// Test objects
	Tile m_TestTile;
	Projectile m_TestProjectile;

public:
	World();
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	~World() 
	{	
		for (auto p : m_pAgents)
			delete p;

		for (auto p : m_pAmmo)
			delete p;
		
		for (auto p : m_pTowers)
			delete p;
	};

	// Inherited virtual functions
	void Render(const IRenderHelpers&) const override;
	void RenderGui(const IGuiHelpers&) override;

	void Update(float pDeltaTime);
	
	// Map functions
	int GetGridHeight(int, int) const;
	int GetGridObstacle(int, int) const;
	vec3 GetGridColour(int, int) const;
	Tile& GetTerrain();
	Tile& GetLeftWall();
	Tile& GetRightWall();
	Tile& GetFrontWall();
	Tile& GetRearWall();
	
	// Time functions
	float GetElapsedTime();
	void SetElapsedTime(float);
	bool TimeUp() const;

	//Agent Wall Detection 
	bool checkForCollisionAgentWall(Agent* a);

	//Ball Wall Detection 
	bool checkForCollisionBallWall(Projectile* b);

	//Sphere Cylinder detection 
	bool sphereCylinderCollision(Projectile* b);

	//Sphere Sphere Collision 
	void sphereSphereCollision(Projectile* b1, Projectile* b2);

	// Agent Sphere Detection 
	void agentSphereCollision(Agent* a, Projectile* b);

	// Gameplay functions
	void UpdateScore();
	vector<Agent*> GetPlayers(int id) 
	{ 
		vector<Agent*> players;
		for (auto i : m_pAgents)
		{
			if(i->GetTeamId() == id)
				players.push_back(i);
		}
		return players; 
	};
	vector<Tower*> GetTowers()
	{
		return m_pTowers;
	};

	vec3 GetSpawnLocations(int id)
	{
		vector<vec3> locations = _LOC_SPAWN_BASES;
		return locations[id];
	}
};

