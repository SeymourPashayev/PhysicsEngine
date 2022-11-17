#ifndef SYSTEM_HPP
#define SYSTEM_HPP

// Own Classes
#include "../Physics/Vec2.hpp"
#include "../Physics/Particle2D.hpp"
#include "../Physics/Force.hpp"
#include "../Physics/Constants.hpp"
#include "../Graphics/Graphics.hpp"
#include "../UI/SceneDevMenu.hpp"
#include "../Inputs/Mouse.hpp"

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

    // Mouse object
    Mouse* mouse;

    // Screen Variabels
    Uint32 screenColour = 0x000000FF;
    //SDL_Renderer *renderer;

    // Developer Menu 
    //SceneDevMenu* devMenu;

    // ------------------------------
    // System Switches
    // ------------------------------
    
    // ---- Simulation Switches ----
    bool GRAVITY_ENABLED = false;
    bool ATTRACTION_ENABLED = true;
    bool FRICTION_ENABLED = true;
    bool DRAG_ENABLED = false;
    bool PARTICLE_COLLISION_ENABLED = true;
    bool SCREEN_COLLISION_ENABLED = true;

    // ---- Debug Switches ----
    bool DEV_MENU_ENABLED = false;
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
    void ApplyForceToParticleOnClick(); // not implemented

    // Switch Toggles
    void ToggleGravity();
    void ToggleAttraction(); //not implemented
    void ToggleFriction(); //not implemented
    void ToggleDrag(); //not implemented
    void ToggleParticleCollision(); //not implemented
    void ToggleScreenCollision(); //not implemented

};

#endif
