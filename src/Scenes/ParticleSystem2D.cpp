// ParticleSystem2D.cpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// This is a simple 2D particle system scene.

// Project Includes
#include "ParticleSystem2D.hpp"
#include "../Physics/Particle2D.hpp"
#include "../Physics/Force.hpp"
#include "../Physics/Constants.hpp"

// System Includes
#include <iostream>

ParticleSystem2D::ParticleSystem2D(Mouse* mouse) {

    this->mouse = mouse;
    
    std::cout << "Particle System Initiated" << std::endl;
    
    // Vector of particles
    std::vector<Particle2D> particles;

    // Liquid Definition and Location is Temporary. Will be moved to a separate class
    // Liquid Setup
    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;

}

ParticleSystem2D::~ParticleSystem2D() {
    
    std::cout << "Particle System Destroyed" << std::endl;

    // Delete all particle references
    for (auto &particle : particles) {
        if (particle != nullptr) {
            delete particle;
            particle = nullptr;

            particleCount--;
        }
    }

}

void ParticleSystem2D::Draw() {
    
    // Draw the liquid
    // Graphics::DrawFillRect(liquid.x + liquid.w/2, liquid.y + liquid.h/2, liquid.w, liquid.h, 0xFF6E3713); 

    for (auto particle: particles){
        if (particle != nullptr) {
            Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, particle->color);
            
            // Draw the velocity vector
            // Create a unit vector representation
            Vec2 unitDirection = particle->velocity.UnitVector()*particle->radius*2;

            Graphics::DrawLine(particle->position.x, particle->position.y, particle->position.x + unitDirection.x, particle->position.y + unitDirection.y, 0xFF000000);
        }
    }

}

void ParticleSystem2D::Update(float dt, Vec2 pushForce) {
   
    // Generate and Add Forces to all the particles
    for (auto particle: particles) {

        // Arrow Key Push Force
        particle->AddForce(pushForce);

        // Gravity
        // particle->AddForce(Force::GenerateWeightForce(*particle));

        // Friction Force
        particle->AddForce(Force::GenerateFrictionForce(*particle, 0.05f));

        // Attrattion Force for all particles
        for (auto otherParticle: particles) {
            if (otherParticle != particle && otherParticle != nullptr) {
                Vec2 attraction = Force::GenerateGravitationalForce(*particle, *otherParticle, GRAVITY, particle->radius + otherParticle->radius, 1000.0f);
                particle->AddForce(attraction);
                otherParticle->AddForce(-attraction);
            }
        }

        // Drag Force, if the particle is in the liquid
        //if (particle->position.y > liquid.y) {
        //    particle->AddForce(Force::GenerateDragForce(*particle, 0.09f));
        //}

    }

    // Integrate all the particles
    for (auto particle : particles) {
        particle->VerletIntegrate(dt);
        // Alternative Integration: Euler Integration, use one at a time
        // particle->EulerIntegrate(dt);
    }

    // Check for collisions
    CheckForScreenCollisions();

    // Check for Particle Collisions
    CheckForParticleCollisions();

    // Create a new particle if the mouse is clicked
    if (mouse->GetLeftClick() == true) {
        CreateParticleAtMouse();
        mouse->SetLeftClick(false);
    }

}

void ParticleSystem2D::CheckForScreenCollisions(){
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

// Create a particle with mouse click at mouse position
void ParticleSystem2D::CreateParticleAtMouse() {
    
    if (mouse->GetLeftClick() == true) {

        Vec2 mousePos = mouse->GetPosition();
        particles.push_back(new Particle2D(mousePos.x, mousePos.y, mass, radius));
        
        // Add to the count of particles
        particleCount++;

        mass = 3.0f;
        radius = 25.0f;
    }
}

// Checks for collisions between every particle, resolves if there is a collision
void ParticleSystem2D::CheckForParticleCollisions() {
    for (int i = 0; i < this->particleCount; i++) {
        for (int j = i + 1; j < this->particleCount; j++) {
            if (particles[i]->CheckCollision(*particles[j])) {
                particles[i]->ResolveCollision(*particles[j]);
            }
        }
    }
}

