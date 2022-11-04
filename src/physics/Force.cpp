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
        float dragMagnitude = dragCoefficient * particle.velocity.MagnitudeSquared() * area;

        // Generate the final drag force with direction and magnitude
        dragForce = dragDirection * dragMagnitude;
    
    }

    return dragForce;

}
