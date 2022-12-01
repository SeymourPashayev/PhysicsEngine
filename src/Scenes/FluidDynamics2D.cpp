// ParticleSystem2D.cpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// This is a simple 2D particle system scene.

// Project Includes
#include "FluidDynamics2D.hpp"


FluidDynamics2D::FluidDynamics2D(Mouse* mouse):System { mouse } {

    std::cout << "Dynamic System Initiated" << std::endl;

}

FluidDynamics2D::~FluidDynamics2D() {

    std::cout << "Dynamic System Destroyed" << std::endl;

}

void FluidDynamics2D::Update(float dt, Vec2 pushForce) {
    
    System::Update(dt, pushForce);

    // Find a way to create an inlfow of liquid at the mous eposition  
}

// Create a particle with mouse click at mouse position
void FluidDynamics2D::CreateParticleAtMouse(float dt) {
  
    float mass = 5.0f;
    float radius = 10.0f;


    if (mouse->GetLeftClick() == true) {

        Vec2 mousePos = mouse->GetPosition();
        particles.push_back(new Particle2D(mousePos.x, mousePos.y, mass, radius));
    
        // Add to the count of particles
        particleCount++;

    }

}
