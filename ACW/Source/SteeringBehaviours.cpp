#include "SteeringBehaviours.h"
#include "Agent.h"
#include "World.h"

SteeringBehaviours::SteeringBehaviours(Agent* pAgent) :
	m_pAgent(pAgent),
	m_BoundingBoxLength(5.0f),
	m_WeightObstacleAvoidance(0.0f),
	m_WeightWander(0.0f),
	m_WeightWallAvoidance(0.0f),
	m_ViewDistance(10.0f),
	m_FeelerLength(5.0f),
	m_Feelers(4),
	m_Deceleration(normal),
	m_pTargetAgent(nullptr),
	m_WanderDistance(WanderDist),
	m_WanderJitter(WanderJitterPerSec),
	m_WanderRadius(WanderRad),
	m_WaypointSeekDistSq(WaypointSeekDist* WaypointSeekDist),
	m_WeightSeek(0.9f),
	m_WeightFlee(0.1f),
	m_WeightArrive(0.0f),
	m_WeightFollowPath(0.0f)
{
	//stuff for the wander behavior

	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = 360.0f;
	float r = random * diff;
	float theta = r * 2 * 3.14159;

	//create a vector to a target position on the wander circle
	m_WanderTarget = vec3(m_WanderRadius * cos(theta), 1.0f, m_WanderRadius * sin(theta));

	// create a Path
	// m_pPath = new Path();
	// m_pPath->LoopOn();
}

SteeringBehaviours::~SteeringBehaviours() { } // delete m_pPath; 

vec3 SteeringBehaviours::Calculate()
{
	//reset the steering force
	m_SteeringForce = vec3(0);

	// m_SteeringForce += WallAvoidance(m_pAgent->GameWorld()->Walls()) * m_WeightWallAvoidance;

	// m_SteeringForce += ObstacleAvoidance(m_pAgent->GameWorld()->Obstacles()) * m_WeightObstacleAvoidance;

	// m_SteeringForce += Wander() * m_WeightWander;

	vec3 pos;
	if (m_pAgent->GetTargetTower() != nullptr)
		pos = m_pAgent->GetTargetTower()->GetPosition();

	m_SteeringForce += Seek(pos) * m_WeightSeek;
	
	m_SteeringForce += Flee(pos) * m_WeightFlee;

	m_SteeringForce += Arrive(pos, m_Deceleration) * m_WeightArrive;

	// m_SteeringForce += FollowPath() * m_WeightFollowPath;

	// m_SteeringForce.Truncate(m_pAgent->MaxForce());

	return m_SteeringForce;
}

vec3 SteeringBehaviours::Seek(vec3 TargetPos)
{
	vec3 DesiredVelocity = normalize(TargetPos - m_pAgent->GetPosition())
		* m_pAgent->GetMaxSpeed();

	return (DesiredVelocity - m_pAgent->GetVelocity());
}

vec3 SteeringBehaviours::Flee(vec3 TargetPos)
{
	vec3 DesiredVelocity = normalize(m_pAgent->GetPosition() - TargetPos)
		* m_pAgent->GetMaxSpeed();

	return (DesiredVelocity - m_pAgent->GetVelocity());
}

vec3 SteeringBehaviours::Arrive(vec3 TargetPos, Deceleration deceleration)
{
	vec3 ToTarget = TargetPos - m_pAgent->GetPosition();

	float distance = length(ToTarget);

	if (distance > 0)
	{
		const float DecelerationTweaker = 0.8f;

		float speed = distance / ((float)deceleration * DecelerationTweaker);

		speed = glm::min(speed, m_pAgent->GetMaxSpeed());

		vec3 DesiredVelocity = ToTarget * speed / distance;

		return (DesiredVelocity - m_pAgent->GetVelocity());
	}

	return vec3(0);
}

vec3 SteeringBehaviours::Wander()
{
	return vec3();
}

vec3 SteeringBehaviours::ObstacleAvoidance(std::vector<GameEntity*>& obstacles)
{
	return vec3();
}

vec3 SteeringBehaviours::WallAvoidance(std::vector<GameEntity*>& walls)
{
	return vec3();
}

vec3 SteeringBehaviours::FollowPath()
{
	return vec3();
}
