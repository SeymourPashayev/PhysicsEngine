#include "Application.h"
#include "./Physics/Constants.hpp"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
    
    particle = new Particle(200, 200, 1.0f, 50.0f);
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
//            case SDL_MOUSEMOTION:
//                int x = event.motion.x;
//                int y = event.motion.y;
//                mouse->UpdatePosition(x, y);
//                break;
//            case SDL_MOUSEBUTTONDOWN:
//                int x, y;
//                SDL_GetMouseState(&x, &y);
//                mouse->UpdatePosition(x, y);
//
//                if (!mouse->GetLeftButtonDown() && event.button.button == SDL_BUTTON_LEFT) {
//                  mouse->SetLeftMouseButton(true);
//                }
//                if (!mouse->GetRightMouseButton() && event.button.button == SDL_BUTTON_RIGHT) {
//                  mouse->SetRightMouseButton(true);
//                }
//                break;
//            case SDL_MOUSEBUTTONUP: 
//            if (mouse->GetLeftButtonDown() && event.button.button == SDL_BUTTON_LEFT) {
//                  mouse->SetLeftMouseButton(false);
//                }
//                if (mouse->GetRightMouseButton() && event.button.button == SDL_BUTTON_RIGHT) {
//                  mouse->SetRightMouseButton(false);
//                }
//                break;
//            case SDL_MOUSEWHEEL:
//                if (event.wheel.y > 0) {
//                  mouse->IncreaseCursorSize(10);
//                } else if (event.wheel.y < 0) {
//                  mouse->IncreaseCursorSize(-10);
//                }
//                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    // Check if we are too fast, and if so, waste time till we reach MILLISECONDS_PER_FRAME
    static int timePreviousFrame;
    
    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0){
        SDL_Delay(timeToWait);
    }

    // Calculate Delta Time in Seconds : use this for displacement instead of pixels to achieve framerate independent movement.
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.0016) {
        deltaTime = 0.0016;
    }


    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

        // ---- Update the Objects in the Scene ----
        
        // Add gravity force
        Vec2 gravity = Vec2(0.0f, 9.81f * PIXELS_PER_METER);
        particle->AddForce(gravity);
        
        // Add wind force
        Vec2 wind = Vec2(1.1f * PIXELS_PER_METER, 0.0f);
        particle->AddForce(wind);

        // Integrate the acceleration and velocity to get the position
        //particle->EulerIntegrate(deltaTime);
        particle->VerletIntegrate(deltaTime);
        
        // Check if the particle has hit the screen boundaries, and if so, reverse the velocity
        if (particle->position.y > Graphics::Height() - particle->radius) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -1;
        }
        if (particle->position.x > Graphics::Width() - particle->radius) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -1;
        }
        if (particle->position.y < particle->radius) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -1;
        }
        if (particle->position.x < particle->radius) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -1;
        }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    
    delete particle;

    Graphics::CloseWindow();
}
