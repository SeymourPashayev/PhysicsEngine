#ifndef SPRINGFORCELATTICE_HPP
#define SPRINGFORCELATTICE_HPP

// Project Includes
#include "../Physics/Vec2.hpp"
#include "../Physics/Particle2D.hpp"
#include "../Graphics.hpp"
#include "../Inputs/Mouse.hpp"

// System Includes
#include <vector>
#include <iostream>

struct SpringForceLattice {
    
    std::vector<Particle2D*> particles;
    int particleCount = 0;

    Mouse* mouse;

    // ------------------------------
    
    SpringForceLattice(Mouse* mouse);
    ~SpringForceLattice();
    
    void Update(float dt, Vec2 pushForce);

    void Draw();

    // Helper Functions
    void CheckForScreenCollisions();
    void CheckForParticleCollisions();

};

#endif
