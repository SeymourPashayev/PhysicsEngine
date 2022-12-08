// SPH.cpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// This is a simple 2D particle system scene.

// Project Includes
#include "SPH.hpp"


SPH::SPH(Mouse* mouse):System { mouse } {

    std::cout << "Dynamic System Initiated" << std::endl;

}

SPH::~SPH() {

    std::cout << "Dynamic System Destroyed" << std::endl;

}

void SPH::Update(float dt, Vec2 pushForce) {
    
    System::Update(dt, pushForce);

    // Find a way to create an inlfow of liquid at the mous eposition  
}

void SPH::CreateParticleAtMouse(float dt) {
  
    float mass = 5.0f;
    float radius = 10.0f;


    if (mouse->GetLeftClick() == true) {

        Vec2 mousePos = mouse->GetPosition();
        particles.push_back(new Particle2D(mousePos.x, mousePos.y, mass, radius));
    
        // Add to the count of particles
        particleCount++;

    }

}
