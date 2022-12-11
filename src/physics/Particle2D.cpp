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

    // std::cout << "2D Particle created with mass: " << mass << std::endl;

}

Particle2D::Particle2D(Vec2 position, float mass, float radius) {
    
    this->position = position;
    
    this->mass = mass;
    if (mass != 0.0) {
        invMass = 1.0 / mass;
    }
    
    this->radius = radius;

    // std::cout << "2D Particle created with mass: " << mass << std::endl;

}

Particle2D::~Particle2D() {

    // std::cout << "2D Particle destructor called." << std::endl;

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
// y' = f(x, y) step function helper for RK4Integrate
Particle2D Particle2D::RK4Step(float dt, Vec2 InPosition, Vec2 InVelocity) {
   
    Particle2D tempParticle = Particle2D(InPosition, this->mass, this->radius);
    tempParticle.velocity = InVelocity;

    // Find the acceleration of the forces being applied
    tempParticle.acceleration = sumForces * invMass;
    
    // Find the velocity of the particle
    tempParticle.velocity = acceleration * dt;

    // Find the position of the particle
    tempParticle.position = velocity * dt;

    // Clear the forces
    ClearForces();
    
    return tempParticle;
}

// Runge-Kutta 4th order integration
void Particle2D::RK4Integrate(float dt) {
   
    // Save the current position to the previous position
    this->prevPosition = this->position;
    
    this->acceleration = sumForces * invMass;

    Particle2D k1 = this->RK4Step(dt, this->position, this->velocity);
    Particle2D k2 = this->RK4Step(0.5f * dt, this->position + k1.position * 0.5f * dt, this->velocity + k1.velocity * 0.5f * dt);
    Particle2D k3 = this->RK4Step(0.5f * dt, this->position + k2.position * 0.5f * dt, this->velocity + k2.velocity * 0.5f * dt);
    Particle2D k4 = this->RK4Step(dt, this->position + k3.position*dt, this->velocity + k3.velocity*dt);
    
    this->position += (k1.position + k2.position * 2.0f + k3.position * 2.0f + k4.position) * (1.0f / 6.0f);
    this->velocity += (k1.velocity + k2.velocity * 2.0f + k3.velocity * 2.0f + k4.velocity) * (1.0f / 6.0f);

    // Clear the forces
    ClearForces();

}

void Particle2D::AddForce(const Vec2& force) {
    
    this->sumForces += force;

}

// Overload AddForce with + operator
void Particle2D::operator+(const Vec2& force) {
    AddForce(force);
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
    
    // The distance between the particles cannot be less than the sum of their radii
    float distance = (this->position - other.position).Magnitude();
    float minDistance = this->radius + other.radius;

    if (distance <= minDistance) {
        // Chance the particles' positions so they are not overlapping
        Vec2 normal = (this->position - other.position).Normalize();
        Vec2 correction = normal * (minDistance - distance);
        this->position += correction * 0.5f;
        other.position -= correction * 0.5f;
    }

    // If the particles have no mass, they can pass through each other 
    if (this->mass == 0.0f && other.mass == 0.0f) {
        return;
    }
    

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


