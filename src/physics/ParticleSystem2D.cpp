#include "ParticleSystem2D.hpp"
#include "Particle2D.hpp"
#include "Force.hpp"

ParticleSystem2D::ParticleSystem2D() {
    
    std::cout << "Particle System Initiated" << std::endl;
    
    // Vector of particles
    std::vector<Particle2D> particles;

    // Create one particle
    particles.push_back(Particle2D(300, 300));

}

ParticleSystem2D::~ParticleSystem2D() {
    
    std::cout << "Particle System Destroyed" << std::endl;

    // Delete all particle references
    for (auto &particle : particles) {
        delete &particle;
    }

}

void ParticleSystem2D::Draw() {
    for (auto particle : particles) {
        Graphics::DrawFillCircle(particle.position.x, particle.position.y, particle.radius, 0XFFFFFF);
    }
}

void ParticleSystem2D::Update(float dt, Vec2 pushForce) {
    
    // Generate and Add Forces to all the particles
    for (auto particle: particles) {
        particle.AddForce(pushForce);
        particle.AddForce(Force::GenerateWeightForce(particle));
        // TODO: Move Liquid Definition and Generate Drag Force
        //particle->ApplyForce(Force::GenerateDragForce(*particle, 0.08f));

    }

    // Integrate all the particles
    for (auto particle : particles) {
        particle.VerletIntegrate(dt);
    }

    // Check for collisions
    CheckForScreenCollisions();

}

void ParticleSystem2D::CheckForScreenCollisions(){
    for (auto particle: particles) {
        if (particle.position.y > Graphics::Height() - particle.radius) {
            particle.position.y = Graphics::Height() - particle.radius;
            particle.velocity.y *= -0.9f;
        }

        if (particle.position.x > Graphics::Width() - particle.radius) {
            particle.position.x = Graphics::Width() - particle.radius;
            particle.velocity.x *= -0.9f;
        }

        if (particle.position.x < particle.radius) {
            particle.position.x = particle.radius;
            particle.velocity.x *= -0.9f;
        }

        if (particle.position.y < particle.radius) {
            particle.position.y = particle.radius;
            particle.velocity.y *= -0.9f;
        }
    }
}

// TODO: Make mouse create a particle
//void ParticleSystem2D::CreateRandomParticleAtMouse(Mouse* mouse) {
//    Particle2D particle = Particle2D(Vec2(mouse->x, mouse->y), Vec2(0.0f, 0.0f));
//    particles.push_back(particle);
//}

