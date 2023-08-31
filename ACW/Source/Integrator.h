#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <iostream>
#include "Config.h"

using namespace glm;
using namespace std;

class Integrator
{
public:
	virtual std::vector<vec3> integrate(float dt, vec3 position, vec3 velocity, float mass, vec3 f1, vec3 f2) = 0;
};

class EulerIntegrator : public Integrator {
public:
    float errThreshold = 1.0f;
    std::vector<vec3> integrate(float dt, vec3 position, vec3 velocity, float mass, vec3 f1, vec3 f2) override {
        cout << "Running Euler" << endl;
        vec3 f, a, v, s;
        vec3 v1, v2;

        float errTruncate, deltaTime;

        f = f1 + f2 - (_DRAG_COEFFICIENT * velocity);

        a = f / mass;
        v1 = velocity + a * dt;

        f = f1 + f2 - (_DRAG_COEFFICIENT * velocity);
        a = f / mass;
        v2 = velocity + a * (dt / 2);

        f = f1 + f2 - (_DRAG_COEFFICIENT * v2);
        a = f / mass;
        v2 = v2 + a * (dt / 2);

        errTruncate = abs(length(v1) - length(v2));
        deltaTime = dt * sqrt(errThreshold / errTruncate); // Tune value 0.05 for error threshold

        if (deltaTime < dt)
        {
            f = f1 + f2 - (_DRAG_COEFFICIENT * velocity);
            a = f / mass;
            v = velocity + a * deltaTime;
            s = position + v * deltaTime;
        }
        else
        {
            v = v1;
            s = position + v * dt;
        }

        std::vector<vec3> update = { s, v };
        return update;
    }
};

class RK4Integrator : public Integrator {
public:
    std::vector<vec3> integrate(float dt, vec3 position, vec3 velocity, float mass, vec3 f1, vec3 f2) override {
        cout << "Running RK4" << endl;
        vec3 f = f1 + f2 - (_DRAG_COEFFICIENT * velocity);
        vec3 a = f / mass;
        // calculate intermediate values
        vec3 k1v = a;
        vec3 k1x = velocity;
        vec3 k2v = a + k1v * (dt / 2);
        vec3 k2x = velocity + k1x * (dt / 2);
        vec3 k3v = a + k2v * (dt / 2);
        vec3 k3x = velocity + k2x * (dt / 2);
        vec3 k4v = a + k3v * dt;
        vec3 k4x = velocity + k3x * dt;
        // update position using weighted average of intermediate values
        position += (k1x + 2.0f * k2x + 2.0f * k3x + k4x) * (dt / 6);
        // update velocity using weighted average of intermediate values
        velocity += (k1v + 2.0f * k2v + 2.0f * k3v + k4v) * (dt / 6);

        std::vector<vec3> update = { position, velocity };
        return update;
    }
};

class VerletIntegrator : public Integrator {
public:
    std::vector<vec3> integrate(float dt, vec3 position, vec3 velocity, float mass, vec3 f1, vec3 f2) override
    {
        cout << "Running Verlet" << endl;
        vec3 f = f1 + f2 - (_DRAG_COEFFICIENT * velocity);
        vec3 a = f / mass;
        vec3 prevPosition = position;
        position += velocity * dt + 0.5f * a * dt * dt;
        velocity += 0.5f * (a)*dt;
        
        std::vector<vec3> update = { position, velocity };
        return update;
    }

private:
    vec3 prevPosition;
};