#ifndef PARTICLESYSTEM2D_HPP
#define PARTICLESYSTEM2D_HPP

// Own Classes
#include "../Physics/Vec2.hpp"
#include "../Physics/Particle2D.hpp"
#include "../Graphics.hpp"
#include "../Inputs/Mouse.hpp"

// System Imports
#include <vector>
#include <iostream>

struct ParticleSystem2D {
    
    std::vector<Particle2D*> particles;
    int particleCount = 0;

    Mouse* mouse;

    // Temporary Variables for the first particle creation
    float mass = 50.0f;
    float radius = 20.0f;

    // ------------------------------
    
    ParticleSystem2D(Mouse* mouse);
    ~ParticleSystem2D();
    
    void Update(float dt, Vec2 pushForce);

    void Draw();

    // Helper Functions
    void CheckForScreenCollisions();
    void CheckForParticleCollisions();

    // Interactions
    void CreateParticleAtMouse();

};

#endif
