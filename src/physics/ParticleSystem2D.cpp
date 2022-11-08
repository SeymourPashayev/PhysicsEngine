#include "ParticleSystem2D.hpp"
#include "Particle2D.hpp"
#include "Force.hpp"

#include <iostream>

ParticleSystem2D::ParticleSystem2D(Mouse* mouse) {
    this->mouse = mouse;
    
    std::cout << "Particle System Initiated" << std::endl;
    
    // Vector of particles
    std::vector<Particle2D> particles;

}

ParticleSystem2D::~ParticleSystem2D() {
    
    std::cout << "Particle System Destroyed" << std::endl;

    // Delete all particle references
    for (auto &particle : particles) {
        if (particle != nullptr) {
            delete particle;
            particle = nullptr;
        }
    }

}

void ParticleSystem2D::Draw() {
    for (Particle2D* particle : particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0XFFFFFF);
    }
}

void ParticleSystem2D::Update(float dt, Vec2 pushForce) {
    
    // Generate and Add Forces to all the particles
    for (auto particle: particles) {
        particle->AddForce(pushForce);
        particle->AddForce(Force::GenerateWeightForce(*particle));
        // TODO: Move Liquid Definition and Generate Drag Force
        //particle->ApplyForce(Force::GenerateDragForce(*particle, 0.08f));

    }

    // Integrate all the particles
    for (auto particle : particles) {
        particle->VerletIntegrate(dt);
    }

    // Check for collisions
    CheckForScreenCollisions();

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
void ParticleSystem2D::CreateRandomParticleAtMouse() {
    if (mouse->GetLeftClick() == true) {
        Vec2 mousePos = mouse->GetPosition();
        particles.push_back(new Particle2D(mousePos.x, mousePos.y));
    }
}

// TODO: Make mouse create a particle
//void ParticleSystem2D::CreateRandomParticleAtMouse(Mouse* mouse) {
//    Particle2D particle = Particle2D(Vec2(mouse->x, mouse->y), Vec2(0.0f, 0.0f));
//    particles.push_back(particle);
//}

