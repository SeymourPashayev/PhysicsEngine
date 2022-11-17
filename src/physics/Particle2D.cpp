// Particle.cpp
// Created: Thu 03 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Particle2D.cpp - A 2D particle struct for the engine

#include "Particle2D.hpp"


// Particle2D Constructor that takes x & y coordinates
Particle2D::Particle2D(float x, float y, float mass, float radius) {
    
    this->position = Vec2(x, y);
    
    this->mass = mass;
    if (mass != 0.0) {
        invMass = 1.0 / mass;
    }
    
    this->radius = radius;

    std::cout << "2D Particle created with mass: " << mass << std::endl;

}

Particle2D::Particle2D(Vec2 position, float mass, float radius) {
    
    this->position = position;
    
    this->mass = mass;
    if (mass != 0.0) {
        invMass = 1.0 / mass;
    }
    
    this->radius = radius;

    std::cout << "2D Particle created with mass: " << mass << std::endl;

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

// ---- Collision Detection ----
bool Particle2D::CheckCollision(Particle2D& other) {
    float distance = (this->position - other.position).Magnitude();
    float minDistance = this->radius + other.radius;
    
    return distance < minDistance;
}

void Particle2D::ResolveCollision(Particle2D& other){

    // Change the particle color if they collide
    // this->CollisionColorImpulse();
    // other.CollisionColorImpulse();

    // Find the normal vector
    Vec2 normal = (other.position - this->position).Normalize();

    // Find the tangent vector
    Vec2 tangent = Vec2(-normal.y, normal.x);

    // Find the dot product of the velocity and the normal
    float dpTan1 = this->velocity.Dot(tangent);
    float dpTan2 = other.velocity.Dot(tangent);

    // Find the dot product of the velocity and the normal
    float dpNorm1 = this->velocity.Dot(normal);
    float dpNorm2 = other.velocity.Dot(normal);

    // Conservation of momentum in 1D
    float m1 = (dpNorm1 * (this->mass - other.mass) + 2.0f * other.mass * dpNorm2) / (this->mass + other.mass);
    float m2 = (dpNorm2 * (other.mass - this->mass) + 2.0f * this->mass * dpNorm1) / (this->mass + other.mass);

    // Update the velocity
    this->velocity = tangent * dpTan1 + normal * m1;
    other.velocity = tangent * dpTan2 + normal * m2;
}


