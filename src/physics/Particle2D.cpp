// Particle.cpp
// Created: Thu 03 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Particle2D.cpp - A 2D particle struct for the engine

#include <iostream>
#include "Particle2D.hpp"
#include "Constants.hpp"

Particle2D::Particle2D(float x, float y, float mass, float radius) {
    this->position = Vec2(x, y);
    
    this->mass = mass;
    if (mass != 0.0) {
        invMass = 1.0 / mass;
    }
    
    this->radius = radius;

    std::cout << "2D Particle constructor caled." << std::endl;
}

Particle2D::~Particle2D() {
    std::cout << "2D Particle destructor called." << std::endl;
}

void Particle2D::EulerIntegrate(float dt) {

    // Save the previous position
    this->prevPosition = this->position;

    // Find the acceleration of the forces being applied
    acceleration = sumForces * invMass;
    
    // Find the velocity of the particle
    velocity += acceleration * dt;

    // Find the position of the particle
    position += velocity * dt;

    // Clear the forces
    ClearForces();
}

// Solve for the position of the particle at the next time step in place
void Particle2D::VerletIntegrate(float dt) {
    
    // Save the current position for the previous position
    //this->prevPosition = this->position;

    Vec2 newPosition = this->position + this->velocity * dt + this->acceleration * dt * dt;
    Vec2 newAcceleration = this->sumForces * invMass;
    Vec2 newVelocity = velocity + (newAcceleration + acceleration) * dt * 0.5f;

    this->position = newPosition;
    this->velocity = newVelocity;
    this->acceleration = newAcceleration;

    // Clear the forces
    ClearForces();

}

void Particle2D::AddForce(const Vec2& force) {
    this->sumForces += force;
}

void Particle2D::ClearForces() {
    this->sumForces = Vec2(0.0f, 0.0f);
}
