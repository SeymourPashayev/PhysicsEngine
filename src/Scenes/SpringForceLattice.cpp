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
    this->PARTICLE_COLLISION_ENABLED = true;
    this->SCREEN_COLLISION_ENABLED = true;


    // Set the Spring Force Lattice size
    width = 4;
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
            Particle2D* particle = new Particle2D { pos, 3.0f, 10.0f };

            // Add the particle to the system
            particles.push_back(particle);

            particleCount++;

            // TODO: Create an AddParticle method that handles particleCount and more for you
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
                Spring *spring = new Spring { particle, particleRight, 1.0f, distanceX }; // TODO Translate to PIXELS_PER_METER
                Springs.push_back(spring);
                springCount++;
            }

            if (particleDown) {
                Spring *spring = new Spring { particle, particleDown, 1.0f, distanceY };
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
void SpringForceLattice::Update(float dt, Vec2 pushForce) {
    
    System::Update(dt, pushForce);
    
}


