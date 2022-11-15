#ifndef APPLICATION_H
#define APPLICATION_H

// Project Includes
#include "./Graphics.hpp"
#include "./Physics/Particle2D.hpp"
#include "./Scenes/ParticleSystem2D.hpp"
#include "./Inputs/Mouse.hpp"

// System Includes
#include <vector>


class Application {
    private:
        
        bool running = false;

        Mouse* mouse;
        ParticleSystem2D* particleSystem;
        
        Vec2 pushForce = Vec2(0.0f, 0.0f);
        
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
