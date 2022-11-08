#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.hpp"
#include "./Physics/Particle2D.hpp"
#include "./Physics/ParticleSystem2D.hpp"
#include "./Inputs/Mouse.hpp"
#include <vector>


class Application {
    private:
        
        bool running = false;

        Mouse* mouse;
        ParticleSystem2D* particleSystem;
        
        Vec2 pushForce = Vec2(0.0f, 0.0f);
        
        SDL_Rect liquid;
        
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
