#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vec2.hpp"

struct Particle {
    
    Vec2 position;
    Vec2 prevPosition;

    Vec2 velocity;
    Vec2 acceleration;

    Vec2 sumForces;

    float mass;
    float radius;
    //float damping;
    
    Particle(float x, float y, float mass=10.0f, float radius=4.0f);
    ~Particle();

    // Integration Methods
    void EulerIntegrate(float dt);
    void VerletIntegrate(float dt);

    void AddForce(const Vec2& force);
    void ClearForces();

}; //ENDOF: Particle


#endif
