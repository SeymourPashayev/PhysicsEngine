// BoundaryParticles.cpp
//
// Created: 2026-01-23
// ------------------------
// Dynamic Boundary Condition (DBC) implementation
// Following research from 02-RESEARCH.md
// ------------------------

#include "BoundaryParticles.hpp"
#include "Vec2.hpp"

// Creates 3-layer rectangular container boundary particles
// Bottom, left, and right walls (top is open for dam break scenarios)
void CreateRectangularContainer(
    std::vector<Particle2D*>& particles,
    float containerWidth,
    float containerHeight,
    float particleSpacing,
    float particleMass
) {
    // Calculate particle radius from spacing (typical for SPH: radius = spacing/2)
    float particleRadius = particleSpacing / 2.0f;

    // Create 3 layers of boundary particles for bottom wall
    // Bottom wall extends slightly beyond container edges for corner coverage
    for (int layer = 0; layer < 3; layer++) {
        float y = -particleSpacing * (layer + 1);  // Below y=0

        // Extend from -3*spacing to containerWidth + 3*spacing
        for (float x = -3.0f * particleSpacing; x <= containerWidth + 3.0f * particleSpacing; x += particleSpacing) {
            Particle2D* boundaryParticle = new Particle2D(x, y, particleMass, particleRadius);
            boundaryParticle->isBoundary = true;
            boundaryParticle->velocity = Vec2(0.0f, 0.0f);
            boundaryParticle->color = 0xFF444444;  // Dark gray for visual distinction
            particles.push_back(boundaryParticle);
        }
    }

    // Create 3 layers of boundary particles for left wall
    for (int layer = 0; layer < 3; layer++) {
        float x = -particleSpacing * (layer + 1);  // Left of x=0

        // From y=0 to containerHeight
        for (float y = 0.0f; y <= containerHeight; y += particleSpacing) {
            Particle2D* boundaryParticle = new Particle2D(x, y, particleMass, particleRadius);
            boundaryParticle->isBoundary = true;
            boundaryParticle->velocity = Vec2(0.0f, 0.0f);
            boundaryParticle->color = 0xFF444444;  // Dark gray for visual distinction
            particles.push_back(boundaryParticle);
        }
    }

    // Create 3 layers of boundary particles for right wall
    for (int layer = 0; layer < 3; layer++) {
        float x = containerWidth + particleSpacing * (layer + 1);  // Right of containerWidth

        // From y=0 to containerHeight
        for (float y = 0.0f; y <= containerHeight; y += particleSpacing) {
            Particle2D* boundaryParticle = new Particle2D(x, y, particleMass, particleRadius);
            boundaryParticle->isBoundary = true;
            boundaryParticle->velocity = Vec2(0.0f, 0.0f);
            boundaryParticle->color = 0xFF444444;  // Dark gray for visual distinction
            particles.push_back(boundaryParticle);
        }
    }

    // Top is intentionally open (no boundary) for dam break scenarios
}
