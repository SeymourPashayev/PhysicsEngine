// WaterDam.cpp
// Created: Thu 22 Dec 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Water Dam Simulation with SPH

// Project Includes
#include "WaterDam.hpp"

// System Includes

WaterDam::WaterDam(Mouse* mouse) : System {mouse} {
    
    // Disable Particle Attraction
    this->ATTRACTION_ENABLED = true;
    this->PARTICLE_COLLISION_ENABLED = true;
    this->SCREEN_COLLISION_ENABLED = true;
    this->MAX_SEARCH_RADIUS = 20.0f;

    // Particle Variables
    float mass = 5.0f;
    float radius = 5.0f;

    // Set the Spring Force Lattice size
    width = 10;
    height = 10;

    // Distance between particles
    float distanceX = 80; // TODO Translate to PIXELS_PER_METER
    float distanceY = 80;

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
}

WaterDam::~WaterDam(){
    System::~System();
}
