// Particle3D.hpp
// Created: Thu 03 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------


#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vec3.hpp"

struct Particle3D {
    
    Vec3 position;
    Vec3 prevPosition;

    Vec3 velocity;
    Vec3 acceleration;

    Vec3 sumForces;

    float mass;
    float invMass;
    
    float radius;
    //float damping;
    
    Particle3D(float x, float y, float z, float mass=10.0f, float radius=4.0f);
    ~Particle3D();

    // Integration Methods
    void EulerIntegrate(float dt);
    void VerletIntegrate(float dt);

    void AddForce(const Vec3& force);
    void ClearForces();

}; //ENDOF: Particle


#endif
