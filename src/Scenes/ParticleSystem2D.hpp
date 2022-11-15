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
    
    // Temporary Variables
    SDL_Rect liquid;
    float mass = 100;
    float radius = 50;

    std::vector<Particle2D*> particles;
    int particleCount = 0;

    Mouse* mouse;

    // ------------------------------
    
    ParticleSystem2D(Mouse* mouse);
    ~ParticleSystem2D();
    
    void Update(float dt, Vec2 pushForce);

    void Draw();
    void DrawForceVector(Particle2D *particle);

    // Helper Functions
    void CheckForScreenCollisions();

    // Interactions
    void CreateParticleAtMouse();
    // Future Idea, TODO: Implement something that uses jerk (3rd derivative). Research applications of jerk calculation in simulations
    
    void CheckForParticleCollisions();
};

#endif
