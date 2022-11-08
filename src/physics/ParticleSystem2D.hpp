#ifndef PARTICLESYSTEM2D_HPP
#define PARTICLESYSTEM2D_HPP

// Own Classes
#include "Vec2.hpp"
#include "Particle2D.hpp"
#include "../Graphics.hpp"
#include "../Inputs/Mouse.hpp"

// System Imports
#include <vector>
#include <iostream>

struct ParticleSystem2D {

    std::vector<Particle2D> particles;

    // ------------------------------
    
    ParticleSystem2D();
    ~ParticleSystem2D();
    
    void Update(float dt, Vec2 pushForce);
    void Draw();

    // Helper Functions
    void CheckForScreenCollisions();

    // Interactions
    void CreateRandomParticleAtMouse(Mouse* mouse);

};

#endif
