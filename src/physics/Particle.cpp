// Particle.cpp
// Created: Thu 03 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Particle.cpp - A particle struct for the engine

#include <iostream>
#include "Particle.hpp"
#include "Constants.hpp"

Particle::Particle(float x, float y, float mass, float radius) {
    this->position = Vec2(x, y);
    this->mass = mass;
    this->radius = radius;

    std::cout << "Particle constructor caled." << std::endl;
}

Particle::~Particle() {
    std::cout << "Particle destructor called." << std::endl;
}

void Particle::EulerIntegrate(float dt) {
    // Find the acceleration of the forces being applied
    acceleration = sumForces / mass;
    
    // Find the velocity of the particle
    velocity += acceleration * dt;

    // Find the position of the particle
    position += velocity * dt;

    // Clear the forces
    ClearForces();
}

// Solve for the position of the particle at the next time step in place
void Particle::VerletIntegrate(float dt) {
    Vec2 newPosition = this->position + this->velocity * dt + this->acceleration * dt * dt;
    Vec2 newAcceleration = this->sumForces / this->mass;
    Vec2 newVelocity = velocity + (newAcceleration + acceleration) * dt * 0.5f;

    this->position = newPosition;
    this->velocity = newVelocity;
    this->acceleration = newAcceleration;

    // Clear the forces
    ClearForces();

}

void Particle::AddForce(const Vec2& force) {
    this->sumForces += force;
}

void Particle::ClearForces() {
    this->sumForces = Vec2(0.0f, 0.0f);
}
