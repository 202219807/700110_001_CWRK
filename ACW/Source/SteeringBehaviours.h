#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "GameEntity.h"

using namespace glm;

class Agent;

const double WanderRad = 1.2;
const double WanderDist = 2.0;
const double WanderJitterPerSec = 80.0;
const double WaypointSeekDist = 20;

class SteeringBehaviours
{
	// Owner instance and steering force
	Agent* m_pAgent;
	vec3 m_SteeringForce;

	// Target 
	Agent* m_pTargetAgent;
	vec3 m_pTargetTowerLocation;

	// Wall and obstacle avoidance
	float m_BoundingBoxLength;

	// Feelers
	std::vector<vec3> m_Feelers;
	float m_FeelerLength;

	// For wandering behaviour
	vec3 m_WanderTarget;
	float m_WanderJitter;
	float m_WanderRadius;
	float m_WanderDistance;

	// multipliers. These can be adjusted to effect strength of the  
	// appropriate behavior. Useful to get flocking the way you require
	// for example.
	float m_WeightWander;
	float m_WeightObstacleAvoidance;
	float m_WeightWallAvoidance;
	float m_WeightSeek;
	float m_WeightFlee;
	float m_WeightArrive;
	float m_WeightFollowPath;

	float m_ViewDistance;

	// Path* m_pPath;

	float m_WaypointSeekDistSq;

	enum Deceleration { slow = 3, normal = 2, fast = 1 };
	Deceleration m_Deceleration;

	bool AccumulateForce(vec3& sf, vec3 ForceToAdd);
	void CreateFeelers();

    //Seek - Moves the agent towards a target position
	vec3 Seek(vec3 TargetPos);

	//Flee - Moves the agent away from a target position
	vec3 Flee(vec3 TargetPos);

	//Arrive - Attempts to arrive at the target position with a zero velocity
	vec3 Arrive(vec3 TargetPos, Deceleration deceleration);

	//Wander - Makes the agent wander about randomly
	vec3 Wander();

	//Obstacle Avoidance - Returns a steering force which will attempt to keep the agent away from any obstacles it may encounter
	vec3 ObstacleAvoidance(std::vector<GameEntity*>& obstacles);

	//Wall Avoidance - Returns a steering force which will keep the agent away from any walls it may encounter
	vec3 WallAvoidance(std::vector<GameEntity*>& walls);

	//Follow Path - Given a series of vec3 points, this method produces a force that will move the agent along the waypoints in order
	vec3 FollowPath();

	
public:
	SteeringBehaviours(Agent*);

	virtual ~SteeringBehaviours();
	
	vec3 Calculate();

	void SetTargetTower(const vec3 tTower) { m_pTargetTowerLocation = tTower; }
	void SetTargetAgent(Agent* tAgent) { m_pTargetAgent = tAgent; }
	// void SetPath(vector<vec3> new_path) { m_pPath->Set(new_path); }
	/*void CreateRandomPath(int num_waypoints, int mx, int my, int mz, int cx, int cy, int cz) const
	{
		m_pPath->CreateRandomPath(num_waypoints, mx, my, mz, cx, cy, cz);
	}*/

	vec3 Force() const { return m_SteeringForce; }
	
	float GetBoundingBoxLength() const { return m_BoundingBoxLength; }
	std::vector<vec3>& GetFeelers() { return m_Feelers; }

	float WanderJitter()const { return m_WanderJitter; }
	float WanderDistance()const { return m_WanderDistance; }
	float WanderRadius()const { return m_WanderRadius; }
};

