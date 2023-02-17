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

    // Set up the Octree screen size
    Vec2 min(0, 0);
    Vec2 max(Graphics::windowWidth, Graphics::windowHeight);
    octree = new Octree2D(particles, min, max, 4); 

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

    // Delete the Octree
    delete octree;

    // Delete ass spring references
    for (auto &spring : Springs) {
        if (spring != nullptr) {
            delete spring;
            spring = nullptr;

            springCount--;
        }
    }

    std::cout << "System Destroyed" << std::endl;

}


void System::Advect(float dt, Vec2 pushForce) {
    
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
        CreateParticleAtMouse();
        mouse->SetLeftClick(false);
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
            //SpringForceCalculatorHelper();
        }

        if (INTEGRATION_ENABLED){
            particle->RK4Integrate(dt);
        }

    }

}


void System::Draw() {
    
    // Clear Screen
    Graphics::ClearScreen(screenColour);

    for (auto particle: particles){
        if (particle != nullptr) {
            Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, particle->color);
        }
    }

}


void System::CheckForScreenCollisions(){
    
    // Too bad it's hardcoded, FIXME
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
void System::CreateParticleAtMouse() {
   
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
        Particle2D* newParticle = new Particle2D(mousePos.x, mousePos.y, mass, radius);
        
        AddParticle(newParticle);
    }

}

// ---- HELPER FUNCTIONS ----

void System::AttractionForceCalculatorHelper(Particle2D& particle) {
    // Find the neighbors of the particle using the octree
    double searchRadius = particle.radius + MAX_SEARCH_RADIUS;
    std::vector<Particle2D*> neighbors;
    octree->findNeighbors(neighbors,&particle, searchRadius);
    
    // Compute the gravitational forces between the particle and its neighbors
    for (int i = 0; i < (int) neighbors.size(); i++) {
        Particle2D* otherParticle = neighbors[i];
        Vec2 attraction = Force::GenerateGravitationalForce(particle, *otherParticle, GRAVITY, particle.radius + otherParticle->radius, 1000.0f);
        particle.AddForce(attraction);
        otherParticle->AddForce(-attraction);
    }
}

void System::AddParticle(Particle2D* particle) {
    
    // Add a particle to the particle list
    particles.push_back(particle);

    // Add the particle to the octree data structure
    octree->insert(particle);

    // Add to the count of particles
    particleCount++;
}

// SPH Functions

// Kernel Function: The function that determines the influence of a particle on another particle FIXME I did not design this and idk if it works well
float System::Kernel(Vec2 r, float h) {
    float q = r.Magnitude() / h;
     if (q > 2) {
        return 0;
    } else if (q > 1) {
        return 3.0f/4.0f - q*q + 0.5f*q*q*q;
    } else {
        return 1 - q*q*(1.5f - 0.5f*q);
    }
}


// ---- SWITCH TOGGLES ----

void System::ToggleIntegration() {
    INTEGRATION_ENABLED = !INTEGRATION_ENABLED;
}

void System::ToggleFluidDynamics() {
    FLUID_DYNAMICS_ENABLED = !FLUID_DYNAMICS_ENABLED;
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


