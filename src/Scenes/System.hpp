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
    
    // ------------------------------
    // System Variables
    // ------------------------------
    
    std::vector<Particle2D*> particles;
    int particleCount = 0;

    Mouse* mouse;

    Uint32 screenColour = 0x000000FF;

    // ------------------------------
    // System Switches
    // ------------------------------

    bool GRAVITY_ENABLED = false;
    bool ATTRACTION_ENABLED = true;
    bool FRICTION_ENABLED = true;
    bool DRAG_ENABLED = false;
    bool PARTICLE_COLLISION_ENABLED = true;
    bool SCREEN_COLLISION_ENABLED = true;
    bool MOUSE_ENABLED = true;
    bool KEYBOARD_ENABLED = true;

    // ------------------------------
    // System Functions
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
    void ApplyForceToParticleOnClick();

};

#endif
