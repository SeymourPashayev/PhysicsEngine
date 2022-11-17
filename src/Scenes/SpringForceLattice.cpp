// ParticleSystem2D.cpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// This is a simple 2D Spring Force Parcile Lattice system scene.

// Project Includes
#include "SpringForceLattice.hpp"

SpringForceLattice::SpringForceLattice(Mouse* mouse) : System { mouse } {
    
    // Disable Particle Attraction
    this->ATTRACTION_ENABLED = false;

    // Set the Spring Force Lattice size
    width = 2;
    height = 1;

    // Offsets for Particle Spawn Position
    float offsetX = Graphics::windowWidth/2.0f;
    float offsetY = Graphics::windowHeight/4.0f;

    // Create particles and add them to the system
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            // Calculate the position of the particle
            Vec2 pos = Vec2 { (i * 50.0f) + offsetX, (j * 50.0f) + offsetY };

            // Create the particle
            Particle2D* particle = new Particle2D { pos, 1.0f, 10.0f };

            // Add the particle to the system
            particles.push_back(particle);

            particleCount++;
        }
    }

    std::cout << "Spring Force Lattice System Initiated" << std::endl;
    
}

// Destructor
SpringForceLattice::~SpringForceLattice() {
    
    System::~System();
    std::cout << "Spring Force Lattice System Destroyed" << std::endl;

}

// Update the system
void SpringForceLattice::Update(float dt, Vec2 pushForce) {
    
    // Create Spring Forces between adjacent particles
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            // Get the current particle
            Particle2D* particle = particles[i * width + j];

            // Get the adjacent particles
            Particle2D* particleLeft = nullptr;
            Particle2D* particleRight = nullptr;
            Particle2D* particleUp = nullptr;
            Particle2D* particleDown = nullptr;

            // Check if the adjacent particles exist
            if (i > 0) {
                particleLeft = particles[(i - 1) * width + j];
            }
            if (i < width - 1) {
                particleRight = particles[(i + 1) * width + j];
            }
            if (j > 0) {
                particleUp = particles[i * width + (j - 1)];
            }
            if (j < height - 1) {
                particleDown = particles[i * width + (j + 1)];
            }

            // Create the spring forces
            if (particleLeft) {
                Vec2 springForce = Force::GenerateSpringForce(*particle, *particleLeft, 50.0f, 100.0f);
                particle->AddForce(springForce);
                particleLeft->AddForce(-springForce);
            }

            if (particleRight) {
                Vec2 springForce = Force::GenerateSpringForce(*particle, *particleRight, 50.0f, 100.0f);
                particle->AddForce(springForce);
                particleRight->AddForce(-springForce);
            }

            if (particleUp) {
                Vec2 springForce = Force::GenerateSpringForce(*particle, *particleUp, 50.0f, 100.0f);
                particle->AddForce(springForce);
                particleUp->AddForce(-springForce);
            }

            if (particleDown) {
                Vec2 springForce = Force::GenerateSpringForce(*particle, *particleDown, 50.0f, 100.0f);
                particle->AddForce(springForce);
                particleDown->AddForce(-springForce);
            }

        }
    }


    System::Update(dt, pushForce);
}


