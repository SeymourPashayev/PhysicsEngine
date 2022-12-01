// ParticleSystem2D.cpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// This is a simple 2D particle system scene.

// Project Includes
#include "System.hpp"


System::System(Mouse* mouse) {

    // Set up the mouse object
    this->mouse = mouse;

    // Set up the renderer
    //this->renderer = renderer;

    // Create a new dev menu
    // Update: The text box I am using is too resource heavy, need to re-write the Text.cpp class
    //devMenu = new SceneDevMenu(Graphics::renderer);
    
    std::cout << "System Initiated" << std::endl;
    
}


System::~System() {
    
    // Delete all particle references
    for (auto &particle : particles) {
        if (particle != nullptr) {
            delete particle;
            particle = nullptr;

            particleCount--;
        }
    }

    // Delete ass spring references
    for (auto &spring : Springs) {
        if (spring != nullptr) {
            delete spring;
            spring = nullptr;

            springCount--;
        }
    }

    // Delete the dev menu
    //delete devMenu;

    std::cout << "System Destroyed" << std::endl;

}


void System::Update(float dt, Vec2 pushForce) {
    
    // Update Developer Menu Text
    if (DEV_MENU_ENABLED) {
        //devMenu->Update();
    }

    // Generate and Add Forces to all the particles
    for (auto particle: particles) {
        
        
        // Arrow Key Push Force
        if (KEYBOARD_ENABLED) {
            particle->AddForce(pushForce);
        }

        // Gravity
        if (GRAVITY_ENABLED){
            particle->AddForce(Force::GenerateWeightForce(*particle));
        }

        // Friction Force
        if (FRICTION_ENABLED){
            particle->AddForce(Force::GenerateFrictionForce(*particle, 0.05f));
        }

        // Attraction Force
        if (ATTRACTION_ENABLED){
           AttractionForceCalculatorHelper(*particle);
        }

        // Drag Force
        if (DRAG_ENABLED){
            particle->AddForce(Force::GenerateDragForce(*particle, 0.09f));
        }

        // Apply sping forces between particles
        if (SPRINGFORCE_ENABLED){
            SpringForceCalculatorHelper();
        }

    }

    // Check for collisions
    if (SCREEN_COLLISION_ENABLED){
        CheckForScreenCollisions();
    }

    // Check for Particle Collisions
    if (PARTICLE_COLLISION_ENABLED){
        CheckForParticleCollisions();
    }

    // Create a new particle if the mouse is clicked
    if (mouse->GetLeftClick() == true && MOUSE_ENABLED) {
        CreateParticleAtMouse(dt);
        mouse->SetLeftClick(false);
    }

    
    // Integrate all the particles
    if (INTEGRATION_ENABLED){
        for (auto particle : particles) {
            particle->VerletIntegrate(dt);
            // Alternative Integration: Euler Integration, use one at a time
            //particle->EulerIntegrate(dt);
        }
    }

}


void System::Draw() {
    
    // Clear Screen
    Graphics::ClearScreen(screenColour);

     // Draw Developer Menu
    if (DEV_MENU_ENABLED) {
        //devMenu->Draw();
    }

    // Draw the spring forces
    if (SPRINGFORCE_ENABLED){
        for (auto *spring : Springs) {
            if (spring != nullptr) {
                Graphics::DrawLine(spring->p1->position.x, spring->p1->position.y, spring->p2->position.x, spring->p2->position.y, 0x303F3F);
            }
        }
    }

    for (auto particle: particles){
        if (particle != nullptr) {
            Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, particle->color);
        }
    }

}


void System::CheckForScreenCollisions(){

    for (auto particle: particles) {
        if (particle->position.y > Graphics::Height() - particle->radius) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -0.9f;
        }

        if (particle->position.x > Graphics::Width() - particle->radius) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -0.9f;
        }

        if (particle->position.x < particle->radius) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.9f;
        }

        if (particle->position.y < particle->radius) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.9f;
        }
    }

}


// Checks for collisions between every particle, resolves if there is a collision
void System::CheckForParticleCollisions() {

    for (int i = 0; i < this->particleCount; i++) {
        for (int j = i + 1; j < this->particleCount; j++) {
            if (particles[i]->CheckCollision(*particles[j])) {
                particles[i]->ResolveCollision(*particles[j]);
            }
        }
    }

}


// Create a particle with mouse click at mouse position
void System::CreateParticleAtMouse(float dt) {
   
    float mass;
    float radius;

    if (particleCount == 0) {
        mass = 200.0f;
        radius = 20.0f;
    } else {
        mass = 5.0f;
        radius = 5.0f;
    }

    if (mouse->GetLeftClick() == true) {

        Vec2 mousePos = mouse->GetPosition();
        particles.push_back(new Particle2D(mousePos.x, mousePos.y, mass, radius));
        
        // Add to the count of particles
        particleCount++;

    }

}

void System::ApplyForceToParticleOnClick(){
   // TODO: Implement this
}

// ---- HELPER FUNCTIONS ----

// A helper function to calculate the attraction force between particles
void System::AttractionForceCalculatorHelper(Particle2D& particle) {
 
        for (auto otherParticle: particles) {
            if (otherParticle != &particle && otherParticle != nullptr) {
                Vec2 attraction = Force::GenerateGravitationalForce(particle, *otherParticle, GRAVITY, particle.radius + otherParticle->radius, 1000.0f);
                particle.AddForce(attraction);
                otherParticle->AddForce(-attraction);
            }
        }
    
}

// A helper function to calculate the spring force between particles, if it exists
void System::SpringForceCalculatorHelper() {
    
    for (auto spring: Springs) {
        if (spring != nullptr) {
            spring->Update();
        }
    }

}


// ---- SWITCH TOGGLES ----

void System::ToggleIntegration() {
    INTEGRATION_ENABLED = !INTEGRATION_ENABLED;
}

void System::ToggleGravity() {
    GRAVITY_ENABLED = !GRAVITY_ENABLED;
}

void System::ToggleFriction() {
    FRICTION_ENABLED = !FRICTION_ENABLED;
}

void System::ToggleAttraction() {
    ATTRACTION_ENABLED = !ATTRACTION_ENABLED;
}

void System::ToggleDrag() {
    DRAG_ENABLED = !DRAG_ENABLED;
}

void System::ToggleSpringForce() {
    SPRINGFORCE_ENABLED = !SPRINGFORCE_ENABLED;
}

void System::ToggleScreenCollision() {
    SCREEN_COLLISION_ENABLED = !SCREEN_COLLISION_ENABLED;
}

void System::ToggleParticleCollision() {
    PARTICLE_COLLISION_ENABLED = !PARTICLE_COLLISION_ENABLED;
}


