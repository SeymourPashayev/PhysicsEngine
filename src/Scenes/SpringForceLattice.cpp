// ParticleSystem2D.cpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// This is a simple 2D Spring Force Parcile Lattice system scene.

// Project Includes
#include "SpringForceLattice.hpp"
#include "../Physics/Particle2D.hpp"
#include "../Physics/Force.hpp"
#include "../Physics/Constants.hpp"

// System Includes
#include <iostream>

SpringForceLattice::SpringForceLattice(Mouse* mouse) {

    std::vector<Particle2D> particles;
    this->mouse = mouse;

    std::cout << "Spring Force Lattice System Initiated" << std::endl;
    
}

SpringForceLattice::~SpringForceLattice() {
    
    // Delete all particle references
    for (auto &particle : particles) {
        if (particle != nullptr) {
            delete particle;
            particle = nullptr;

            particleCount--;
        }
    }

    std::cout << "Spring Force Lattice System Destroyed" << std::endl;
}

void SpringForceLattice::Draw() {
    
    for (auto particle: particles){
        if (particle != nullptr) {
            Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, particle->color);
        }
    }

}

void SpringForceLattice::Update(float dt, Vec2 pushForce) {
   
    // Generate and Add Forces to all the particles
    for (auto particle: particles) {

        // Arrow Key Push Force
        particle->AddForce(pushForce);

        // Gravity
        particle->AddForce(Force::GenerateWeightForce(*particle));

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

}

void SpringForceLattice::CheckForScreenCollisions(){
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
void SpringForceLattice::CheckForParticleCollisions() {
    for (int i = 0; i < this->particleCount; i++) {
        for (int j = i + 1; j < this->particleCount; j++) {
            if (particles[i]->CheckCollision(*particles[j])) {
                particles[i]->ResolveCollision(*particles[j]);
            }
        }
    }
}

