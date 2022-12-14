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
    this->PARTICLE_COLLISION_ENABLED = false;
    this->SCREEN_COLLISION_ENABLED = true;

    // Particle Variables
    float mass = 3.0f;
    float radius = 10.0f;

    // Set the Spring Force Lattice size
    width = 2;
    height = 4;

    // Distance between particles
    float distanceX = 50; // TODO Translate to PIXELS_PER_METER
    float distanceY = 50;

    // Offsets for Particle Spawn Position
    float offsetX = Graphics::windowWidth/2.0f - (width * distanceX/2.0f);
    float offsetY = Graphics::windowHeight/2.0f - (height * distanceY/2.0f);

    // Create particles and add them to the system
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            // Calculate the position of the particle
            Vec2 pos = Vec2 { (i * distanceX ) + offsetX, (j * distanceY ) + offsetY };

            // Create the particle
            Particle2D* particle = new Particle2D { pos, mass, radius };
            
            AddParticle(particle);
        }
    }

    // Create springs for adjacent particles and add them to the system
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            // Get the current particle
            Particle2D* particle = particles[i + j * width];

            // Get the adjacent particles
            Particle2D* particleRight = (i < width - 1) ? particles[(i + 1) + j * width] : nullptr;
            Particle2D* particleDown = (j < height - 1) ? particles[i + (j + 1) * width] : nullptr;

            // Create the springs
            if (particleRight) {
                Spring *spring = new Spring { particle, particleRight, 0.01f, distanceX + particle->radius + particleRight->radius }; // TODO Translate to PIXELS_PER_METER
                Springs.push_back(spring);
                springCount++;
            }

            if (particleDown) {
                Spring *spring = new Spring { particle, particleDown, 0.01f, distanceY + particle->radius + particleDown->radius }; // TODO Translate to PIXELS_PER_METER
                Springs.push_back(spring);
                springCount++;
            }
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
void SpringForceLattice::Advect(float dt, Vec2 pushForce) {
    
    System::Advect(dt, pushForce);
    
}


