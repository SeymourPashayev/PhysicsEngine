// Force.cpp
// Created: Fri 04 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------ 
// Helper file to generate forces on particles
//

#include "Force.hpp"

Vec2 Force::GenerateDragForce(const Particle2D& particle, float dragCoefficient, float area) {
    
    Vec2 dragForce = Vec2(0, 0);

    if (particle.velocity.MagnitudeSquared() > 0) {

        // Calculate the drag direction
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;

        // Calculate the magnitude of the drag force, k * v^2 * A
        float dragMagnitude = dragCoefficient * particle.velocity.MagnitudeSquared() * area;// * PIXELS_PER_METER;?? TODO: WHY DO WE NOT MULTIPLY BY PIXELS_PER_METER? Because it is already in pixels? The function and the magnitude are already 

        // Generate the final drag force with direction and magnitude
        dragForce = dragDirection * dragMagnitude;
    
    }

    return dragForce;

}

Vec2 Force::GenerateWeightForce(const Particle2D& particle) {
    return Vec2(0.0f, particle.mass * GRAVITY * PIXELS_PER_METER);
}

Vec2 Force::GenerateFrictionForce(const Particle2D& particle, float frictionCoefficient) {

    Vec2 frictionForce = Vec2(0, 0);

    if (particle.velocity.MagnitudeSquared() > 0) {

        // Calculate the friction direction
        Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0;

        // Calculate the magnitude of the friction force (mu * N)
        float frictionMagnitude = frictionCoefficient * particle.mass * GRAVITY * PIXELS_PER_METER;

        // Generate the final friction force with direction and magnitude
        frictionForce = frictionDirection * frictionMagnitude;
    
    }

    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Particle2D& particle1, const Particle2D& particle2, float gravitationalConstant) {

    // Calculate the direction of the force
    Vec2 direction = particle2.position - particle1.position;

    // Calculate the magnitude of the force
    float magnitude = gravitationalConstant * (particle1.mass * particle2.mass) / (direction.MagnitudeSquared()) * PIXELS_PER_METER;

    // Calculate the force vector
    Vec2 force = direction.UnitVector() * magnitude;

    return force;
}
