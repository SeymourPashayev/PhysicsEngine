#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vec2.hpp"

struct Particle2D {
    
    Vec2 position;
    Vec2 prevPosition;

    Vec2 velocity;
    Vec2 acceleration;

    Vec2 sumForces;

    float mass;
    float invMass;
    
    float radius;
    //float damping;
    
    Particle2D(float x, float y, float mass=10.0f, float radius=40.0f);
    ~Particle2D();

    // Integration Methods
    void EulerIntegrate(float dt);
    void VerletIntegrate(float dt);

    // Force Calculations
    void AddForce(const Vec2& force);
    void ClearForces();

}; //ENDOF: Particle


#endif
