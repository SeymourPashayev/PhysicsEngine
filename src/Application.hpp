// Application.hpp
// Created: Thu 3 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------


#ifndef APPLICATION_H
#define APPLICATION_H

// Project Includes
#include "./Graphics/Graphics.hpp"
#include "./Physics/Particle2D.hpp"
#include "./Inputs/Mouse.hpp"
#include "./Scenes/ParticleSystem2D.hpp"
#include "./Scenes/SpringForceLattice.hpp"
#include "./Physics/Force.hpp"
#include "./Physics/Constants.hpp"

// System Includes
#include <vector>


class Application {

    private:
        
        bool running = false;
        Mouse* mouse;

        Vec2 pushForce = Vec2(0.0f, 0.0f);

        // The system/set in use.
        //ParticleSystem2D* particleSystem;
        //SpringForceLattice* springForceSystem;
        System* system;
        
    public:
        
        Application() = default;
        ~Application() = default;
    
        bool IsRunning();
        void Setup();
    
        void Input();
        void Update();
        void Render();
    
        void Destroy();

};

#endif
