#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.hpp"
#include "./Physics/Particle2D.hpp"
#include <vector>


class Application {
    private:
        bool running = false;
        
        std::vector<Particle2D*> particles;
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