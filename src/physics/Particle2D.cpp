// Particle.cpp
// Created: Thu 03 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Particle2D.cpp - A 2D particle struct for the engine

// Project Includes
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

// Simple Euler Integration 
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
// Verlet Integration is a better method than Euler Integration
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

// Runge-Kutta 4th order integration
void Particle2D::RK4Integrate(float dt) {
    
    // Save the current state of the particle
    Vec2 pos = this->position;
    Vec2 vel = this->velocity;
    Vec2 acc = this->acceleration;

    // Compute the intermediate values for the Runge-Kutta method
    Vec2 k1_pos = vel;
    Vec2 k1_vel = acc;
    Vec2 k2_pos = vel + k1_vel * (dt / 2);
    Vec2 k2_vel = acc + k1_vel * (dt / 2);
    Vec2 k3_pos = vel + k2_vel * (dt / 2);
    Vec2 k3_vel = acc + k2_vel * (dt / 2);
    Vec2 k4_pos = vel + k3_vel * dt;
    Vec2 k4_vel = acc + k3_vel * dt;

    // Update the position, velocity, and acceleration of the particle
    this->position = (pos + (k1_pos + k2_pos * 2.00 + k3_pos * 2.00 + k4_pos) * (dt / 6));
    this->velocity = (vel + (k1_vel + k2_vel* 2.00 + k3_vel * 2.00 + k4_vel) * (dt / 6));
  
    this->acceleration = sumForces * invMass;

    ClearForces();
}

// ----------------------------
// ---- Force Calculations ----
// ----------------------------


// Adds a force to the sumForces vector.
void Particle2D::AddForce(const Vec2& force) {
    
    this->sumForces += force;

}

// Clear the sumForces vector
void Particle2D::ClearForces() {
    this->sumForces = Vec2(0.0f, 0.0f);
}


// Distance Field Calculation
float Particle2D::ComputeDistanceField(){
    
    return 0.0f;
}


// Collision Detection
bool Particle2D::CheckCollision(Particle2D& other) {

    float distance = (this->position - other.position).Magnitude();
    float minDistance = this->radius + other.radius;
    
    return distance < minDistance;

}

// Collision Resolution
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

// ------------------------------
// ---- Operator Overloading ----
// ------------------------------

// Overload AddForce with + operator
void Particle2D::operator+(const Vec2& force) {
    AddForce(force);
}

