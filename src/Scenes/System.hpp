#ifndef SYSTEM_HPP
#define SYSTEM_HPP

// Own Classes
#include "../Physics/Vec2.hpp"
#include "../Physics/Particle2D.hpp"
#include "../Graphics.hpp"
#include "../Inputs/Mouse.hpp"
#include "../Physics/Force.hpp"
#include "../Physics/Constants.hpp"

// System Imports
#include <vector>
#include <iostream>


struct System {
    
    std::vector<Particle2D*> particles;
    int particleCount = 0;

    Mouse* mouse;

    // ------------------------------
    
    System(Mouse* mouse);
    ~System();
    
    void Update(float dt, Vec2 pushForce);
    void Draw();

    // Helper Functions
    void CheckForScreenCollisions();
    void CheckForParticleCollisions();

    // Interactions
    void CreateParticleAtMouse();

};

#endif
