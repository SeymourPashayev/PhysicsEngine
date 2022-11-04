#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Particle2D.hpp"

class Application {
    private:
        bool running = false;
        Particle2D* particle;

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
