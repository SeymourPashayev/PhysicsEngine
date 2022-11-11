#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vec2.hpp"
#include <SDL2/SDL_stdinc.h>

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
    float coefficientOfRestitution = 0.75f;

    // ---- Decorative Variables ----
    Uint32 color = 0xFFFFFFFF;

    // 1) in between 0 to 1, then the colliding bodies are partially elastic.
    // 2) equal to zero, then the colliding bodies are perfectly inelastic.
    // 3) is equal to one, then the colliding bodies are perfectly elastic.
    
    Particle2D(float x, float y, float mass=10.0f, float radius=40.0f);
    ~Particle2D();

    // Integration Methods
    void EulerIntegrate(float dt);
    void VerletIntegrate(float dt);

    // Force Calculations
    void AddForce(const Vec2& force);
    void ClearForces();

    // Collision Detection
    bool CheckCollision(Particle2D& other);
    void ResolveCollision(Particle2D& other);

}; //ENDOF: Particle


#endif
