#pragma once

#include "Physics.h"
#include "Agent.h"

class Physics;
class Agent;

class LocomotionSystem {
public:
    // Initialization function
    void Init(Agent& agent, Physics& physics);

    // Update function to be called every frame
    void Update(float dt);

    // Function to handle walking
    void Walk(const vec3& direction);

    // Function to handle jumping
    void Jump();

private:
    Agent* m_agent;
    Physics* m_physics;
    bool m_isJumping;
};

void LocomotionSystem::Init(Agent& agent, Physics& physics) {
    m_agent = &agent;
    m_physics = &physics;
    m_isJumping = false;
}

void LocomotionSystem::Update(float dt) {
    if (m_isJumping) {
        // Apply upward force to agent
        m_agent->SetForce(vec3(0, 10.0f, 0));

        // Check if agent has landed
        /*if (m_physics->getGroundHeight(m_agent->GetPosition()) > m_agent->GetPosition().y) {
            m_isJumping = false;
        }*/
    }
    else {
        // Apply gravity to agent
        m_agent->SetForce(vec3(0, _GRAVITATIONAL_ACCELERATION, 0));
    }

    // Update agent's position and velocity
    m_agent->Update(dt);
}

void LocomotionSystem::Walk(const vec3& direction) {
    // Apply force to agent in the desired direction
    m_agent->SetForce(direction * 5.0f);
}

void LocomotionSystem::Jump() {
    if (!m_isJumping) {
        m_isJumping = true;
    }
}

