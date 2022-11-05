#include "Application.hpp"

// Temporary Include of Force
#include "./Physics/Force.hpp"

#include "./Physics/Constants.hpp"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
    
    // Liquid Setup
    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;

    Particle2D* smallBall = new Particle2D(50, 100, 5, 5);
    particles.push_back(smallBall);
    
    Particle2D* bigBall = new Particle2D(50, 200, 20, 25);
    particles.push_back(bigBall);

    Particle2D* bigBall2 = new Particle2D(50, 300, 20, 25);
    particles.push_back(bigBall2);

    // Create a 100 random sized balls
   // for (int i = 0; i < 10; i++) {
   //     Particle2D* ball = new Particle2D(50, 400, rand() % 250, rand() % 250);
   //     particles.push_back(ball);
   // }
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
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = -150 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 150 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 150 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -150 * PIXELS_PER_METER;
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
    
    // Add weight force
    for (auto particle: particles){
        Vec2 weight = Vec2(0.0f, particle->mass * 9.81f * PIXELS_PER_METER);
        particle->AddForce(weight);
   
        particle->AddForce(pushForce);
    
        // Add drag force inside the liquid
        if (particle->position.y > liquid.y) {
            Vec2 drag = Force::GenerateDragForce(*particle, 0.08f);
            particle->AddForce(drag);
        }
    }
    
    // Integrate the acceleration and velocity to get the position
    for (auto particle: particles){
        //particle->EulerIntegrate(deltaTime);
        particle->VerletIntegrate(deltaTime);
    }

        
    for (auto particle: particles){
        // Check if the particle has hit the screen boundaries, and if so, reverse the velocity
        if (particle->position.y > Graphics::Height() - particle->radius) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -0.9;
        }
        if (particle->position.x > Graphics::Width() - particle->radius) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -0.9;
        }
        if (particle->position.y < particle->radius) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.9;
        }
        if (particle->position.x < particle->radius) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.9;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
   
    // Draw the liquid
    Graphics::DrawFillRect(liquid.x + liquid.w/2, liquid.y + liquid.h/2, liquid.w, liquid.h, 0xFF6E3713); 

    for(auto particle: particles){
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }
    
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    
    // Do not forget to delete everything that uses the keyword "new" to instantiate
    for(auto particle: particles){
        delete particle;
    }

    Graphics::CloseWindow();
}
