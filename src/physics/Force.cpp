// Force.cpp
// Created: Fri 04 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------ 
// Helper file to generate forces on particles
//

// System Includes
#include <algorithm>

// ProjectIncludes
#include "Force.hpp"
#include "Constants.hpp"

/**
 * @brief Generates a drag force on a particle
 * 
 * @param particle The particle to generate the force on
 * @param dragCoefficient The drag coefficient of the particle
 * @param area The area of the particle
 *
 * @return Vec2 The drag force
 */
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

/**
 * @brief Generates a weight force on a particle
 * 
 * @param particle The particle to generate the force on
 *
 * @return Vec2 The weight force
 */
Vec2 Force::GenerateWeightForce(const Particle2D& particle) {
    return Vec2(0.0f, particle.mass * GRAVITY * PIXELS_PER_METER);
}

/**
 * @brief Generates a friction force on a particle
 * 
 * @param particle The particle to generate the friction force on
 * @param frictionCoefficient The coefficient of friction
 *
 * @return Vec2 The friction force
 */
Vec2 Force::GenerateFrictionForce(const Particle2D& particle, float frictionCoefficient) {

    Vec2 frictionForce = Vec2(0, 0);

    if (particle.velocity.MagnitudeSquared() > 0) {

        // Calculate the friction direction
        Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0;

        // Calculate the magnitude of the friction force (mu * N)
        float frictionMagnitude = frictionCoefficient * particle.mass * GRAVITY;

        // Generate the final friction force with direction and magnitude
        frictionForce = frictionDirection * frictionMagnitude;
    
    }

    return frictionForce;
}

/**
 * @brief Generates a gravitational force between two particles
 * 
 * @param particle1 The first particle
 * @param particle2 The second particle
 * @param gravitationalConstant The gravitational constant
 * @param minDistance The minimum distance between the two particles for the distance to be clipped to
 * @param maxDistance The maximum distance between the two particles for the distance to be clipped to
 *
 * @return Vec2 The Gravitational Force between the two particles (Apply to both particles in opposite directions)
 */
Vec2 Force::GenerateGravitationalForce(const Particle2D& particle1, const Particle2D& particle2, float gravitationalConstant, float minDistance, float maxDistance) {

    // Calculate the direction of the force
    Vec2 direction = particle2.position - particle1.position;

    // Distance Squared clamped to min and max values 
    //(Ignore complier error, ycm must have a lower cpp standard enabled)
    float distanceSquared = std::clamp(direction.MagnitudeSquared(), minDistance, maxDistance);

    // Calculate the magnitude of the force
    float magnitude = gravitationalConstant * (particle1.mass * particle2.mass) / (distanceSquared) * PIXELS_PER_METER;

    // Calculate the force vector
    Vec2 force = direction.UnitVector() * magnitude;

    return force;
}

/**
 * @brief Generates a spring force between a particle and an anchor point
 * 
 * @param particle The particle to generate the force on
 * @param anchor The anchor point
 * @param springConstant The spring constant
 * @param restLength The rest length of the spring
 *
 * @return Vec2 The spring force
 */
Vec2 Force::GenerateSpringForce(const Particle2D& particle, const Vec2& anchor, float springConstant, float restLength) {

    // Calculate the distance between the particle and the anchor
    Vec2 d = particle.position - anchor;

    // Calculate the displacement of the spring
    float displacement = d.Magnitude() - restLength;

    // Calculate force diraction and magnitude
    Vec2 direction = d.UnitVector();
    float magnitude = -springConstant * displacement;

    // Final Resulting Force
    Vec2 force = direction * magnitude;
    return force;
}

/**
 * @brief Generates a spring force between two particles
 * 
 * @param particle1 The first particle
 * @param particle2 The second particle
 * @param springConstant The spring constant
 * @param restLength The rest length of the spring
 *
 * @return Vec2 The spring force
 */
Vec2 Force::GenerateSpringForce(const Particle2D& particle1, const Particle2D& particle2, float springConstant, float restLength) {

    // Calculate the distance between the particle and the anchor
    Vec2 d = particle1.position - particle2.position;

    // Calculate the displacement of the spring
    float displacement = d.Magnitude() - restLength;

    // Calculate force diraction and magnitude
    Vec2 direction = d.UnitVector();
    float magnitude = -springConstant * displacement;

    // Final Resulting Force
    Vec2 force = direction * magnitude;
    return force;
}
