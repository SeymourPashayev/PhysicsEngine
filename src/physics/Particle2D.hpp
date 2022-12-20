// Particle2D.hpp
//
// Created: Thu 3 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------


#ifndef PARTICLE_HPP
#define PARTICLE_HPP

// Project Includes
#include "Vec2.hpp"
#include "Constants.hpp"

// System Includes
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <iostream>


struct Particle2D {

    Vec2 prevPosition;
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    Vec2 sumForces;

    float mass;
    float invMass;
    
    float radius;
    //float damping;
    float coefficientOfRestitution = 0.75f;

    // 1) in between 0 to 1, then the colliding bodies are partially elastic.
    // 2) equal to zero, then the colliding bodies are perfectly inelastic.
    // 3) is equal to one, then the colliding bodies are perfectly elastic.
    
    // ------------------------------
    // ---- Decorative Variables ----
    // ------------------------------
    Uint32 color = 0xFFFFFFFF;


    // --------------------------
    
    Particle2D(float x, float y, float mass = 20.0f, float radius = 20.0f);
    Particle2D(Vec2 position, float mass = 20.0f, float radius = 20.0f);
    ~Particle2D();
    
    // -----------------------------
    // ---- Integration Methods ----
    // -----------------------------
    void EulerIntegrate(float dt);
    void VerletIntegrate(float dt);
    void RK4Integrate(float dt);

    // -----------------------------
    // ---- Force Calculations ----
    // -----------------------------

    // Force Utilities
    void AddForce(const Vec2& force);
    void ClearForces();

    // SPH Forces
    float ComputeDistanceField();

    // Collision Detection
    bool CheckCollision(Particle2D& other);
    void ResolveCollision(Particle2D& other);

    // Operator Overloading
    void operator+(const Vec2& force);

}; //ENDOF: Particle


#endif
