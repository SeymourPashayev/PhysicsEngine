// Particle.cpp
// Created: Thu 03 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Particle3D.cpp - A 3D particle struct for the engine

#include <iostream>
#include "Particle3D.hpp"
#include "Constants.hpp"

Particle3D::Particle3D(float x, float y, float z, float mass, float radius) {
    this->position = Vec3(x, y, z);
    this->mass = mass;
    this->radius = radius;

    std::cout << "3D Particle constructor caled." << std::endl;
}

Particle3D::~Particle3D() {
    std::cout << "3D Particle destructor called." << std::endl;
}

void Particle3D::EulerIntegrate(float dt) {
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
void Particle3D::VerletIntegrate(float dt) {
    Vec3 newPosition = this->position + this->velocity * dt + this->acceleration * dt * dt;
    Vec3 newAcceleration = this->sumForces / this->mass;
    Vec3 newVelocity = velocity + (newAcceleration + acceleration) * dt * 0.5f;

    this->position = newPosition;
    this->velocity = newVelocity;
    this->acceleration = newAcceleration;

    // Clear the forces
    ClearForces();

}

void Particle3D::AddForce(const Vec3& force) {
    this->sumForces += force;
}

void Particle3D::ClearForces() {
    this->sumForces = Vec3(0.0f, 0.0f, 0.0f);
}

