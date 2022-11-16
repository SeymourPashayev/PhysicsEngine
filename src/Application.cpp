// Application.cpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Application class is responsible for the main loop of the project. While there is a main with a more abstracted loop, this file serves a medium for seemless scene/set 
// switching without any change to the main main loop.

// Application Includes
#include "Application.hpp"
#include "./Physics/Force.hpp"
#include "./Physics/Constants.hpp"
#include "./Inputs/Mouse.hpp"



// Function to check if the application is running
bool Application::IsRunning() {
    return running;
}


// ------------------------------------ //
// ---------- Setup Function ---------- //
// ------------------------------------ //
void Application::Setup() {

    // Setu Graphics
    running = Graphics::OpenWindow();
    
    // Setup Mouse and Particle System
    mouse = new Mouse();

    // The system/set currently in use TODO: MAKE an abstract class System to inherit from in Particle and SpringForce Systems
    //particleSystem = new ParticleSystem2D(mouse);
    springForceSystem = new SpringForceLattice(mouse);
    
    
}


// ------------------------------------ //
// --------- Input Processing --------- //
// ------------------------------------ //
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
                    pushForce.y = - 10 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 10 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 10 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -10 * PIXELS_PER_METER;
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


// ------------------------------------ //
// --------- Update Processing -------- //
// ------------------------------------ //
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
    
    // Update the Objects in the Scene
    //particleSystem->Update(deltaTime, pushForce);
    springForceSystem->Update(deltaTime, pushForce);

}


// ----------------------------------- //
// -------------- Render ------------- //
// ----------------------------------- //
void Application::Render() {

    Graphics::ClearScreen(0xFF056263);
   
    //particleSystem->Draw();
    springForceSystem->Draw();

    Graphics::RenderFrame();

}


// ----------------------------------- //
// ------------- Cleanup ------------- //
// ----------------------------------- //
void Application::Destroy() {

    // Delete the mouse
    delete mouse;

    // Delete the particle system
    //delete particleSystem;
    delete springForceSystem;

    // Close the window
    Graphics::CloseWindow();

}
