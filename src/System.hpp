// System.hpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
//

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

// Own Classes
#include "Physics/Vec2.hpp"
#include "Physics/Particle2D.hpp"
#include "Physics/Spring.hpp"
#include "Physics/Force.hpp"
#include "Physics/Constants.hpp"
#include "Physics/Octree2D.hpp"
#include "Graphics/Graphics.hpp"
#include "Inputs/Mouse.hpp"

// System Imports
#include <vector>
#include <iostream>


struct System {
    
    // ------------------------------
    // System Variables
    // ------------------------------
    
    // Particle Variables
    std::vector<Particle2D*> particles;
    int particleCount = 0;

    // Spring Variables
    std::vector<Spring*> Springs;
    int springCount = 0;
    
    // Octree
    Octree2D* octree = nullptr;

    // Mouse object
    Mouse* mouse;

    // Screen Variabels
    // Set screen colour to gray by default
    Uint32 screenColour = 0x0022000;
    //SDL_Renderer *renderer;

    // Hard coded variables
    float MAX_SEARCH_RADIUS = 0.23f * PIXELS_PER_METER;

    // ------------------------------
    // System Switches
    // ------------------------------
    
    // ---- Simulation Switches ----
    bool INTEGRATION_ENABLED = true;
    bool FLUID_DYNAMICS_ENABLED = true;
    bool GRAVITY_ENABLED = false;
    bool ATTRACTION_ENABLED = true;
    bool FRICTION_ENABLED = true;
    bool DRAG_ENABLED = false;
    bool PARTICLE_COLLISION_ENABLED = true;
    bool SCREEN_COLLISION_ENABLED = true;
    bool SPRINGFORCE_ENABLED = true;

    // ---- Debug Switches ----
    bool DEV_MENU_ENABLED = false;
    bool MOUSE_ENABLED = true;
    bool KEYBOARD_ENABLED = true;

    // ------------------------------
    // System Functions
    // ------------------------------

    System(Mouse* mouse);
    ~System();
    
    void Advect(float dt, Vec2 pushForce);
    void Project(float dt);
    void Draw();

    // Boundary/Interaction Functions
    void CheckForScreenCollisions();
    void CheckForParticleCollisions();

    // Force Application
    void AttractionForceCalculatorHelper(Particle2D& particle);


    // ----------------------
    // ---- Interactions ----
    // ----------------------

    void CreateParticleAtMouse();
    void AddParticle(Particle2D* particle);

    // Simulation Switch Toggles
    void ToggleIntegration();
    void ToggleFluidDynamics();
    void ToggleGravity();
    void ToggleAttraction(); 
    void ToggleFriction();
    void ToggleDrag(); 
    void ToggleParticleCollision(); 
    void ToggleScreenCollision();
    void ToggleSpringForce();

};

#endif
