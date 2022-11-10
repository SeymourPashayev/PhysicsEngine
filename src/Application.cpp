#include "Application.hpp"

#include "./Physics/Force.hpp"
#include "./Physics/Constants.hpp"
#include "./Inputs/Mouse.hpp"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {

    // Setu Graphics
    running = Graphics::OpenWindow();
    
    // Setup Mouse and Particle System
    mouse = new Mouse();
    particleSystem = new ParticleSystem2D(mouse);
    
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    Vec2 mousePos;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = - 100 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 100 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 100 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -100 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouse->SetLeftClick(true);
                    // create a partcile
                    particleSystem->CreateParticleAtMouse();
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouse->SetRightClick(true);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouse->SetLeftClick(false);
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouse->SetRightClick(false);
                }
                break;
            case SDL_MOUSEMOTION:
                mousePos.x = event.motion.x;
                mousePos.y = event.motion.y;
                mouse->UpdatePosition(mousePos);
                break;
            case SDL_MOUSEWHEEL:
                if (event.wheel.y > 0) {
                    mouse->IncreaseCursorSize(10);
                } else if (event.wheel.y < 0) {
                    mouse->IncreaseCursorSize(-10);
                }
                break;

        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {

    // TODO: Move delta time calculation to a separate function
    // TODO: Move TTW calculation to a separate function

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
    
    // Update the Objects in the Scene
    particleSystem->Update(deltaTime, pushForce);

}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {

    Graphics::ClearScreen(0xFF056263);
   
    particleSystem->Draw();

    Graphics::RenderFrame();

}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    
    // Delete the mouse
    delete mouse;

    // Delete the particle system
    delete particleSystem;

    Graphics::CloseWindow();

}
