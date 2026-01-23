// BoundaryParticles.cpp
//
// Created: 2026-01-23
// ------------------------
// Dynamic Boundary Condition (DBC) implementation
// ------------------------

#include "BoundaryParticles.hpp"
#include "Vec2.hpp"

// Creates 3-layer rectangular container boundary particles
// Bottom, left, and right walls (top is open for dam break scenarios)
// Boundaries are placed INSIDE the screen bounds so Octree can find them
void CreateRectangularContainer(
    std::vector<Particle2D*>& particles,
    float containerWidth,
    float containerHeight,
    float particleSpacing,
    float particleMass
) {
    float particleRadius = particleSpacing / 2.0f;
    int numLayers = 3;

    // Margin from screen edge to keep boundaries inside Octree bounds
    float margin = particleSpacing;

    // Bottom wall - placed INSIDE screen (3 layers going up from bottom)
    for (int layer = 0; layer < numLayers; layer++) {
        float y = containerHeight - margin - particleSpacing * layer;

        for (float x = 0.0f; x <= containerWidth; x += particleSpacing) {
            Particle2D* p = new Particle2D(x, y, particleMass, particleRadius);
            p->isBoundary = true;
            p->velocity = Vec2(0.0f, 0.0f);
            p->color = 0xFF888888;  // Light gray for visibility
            particles.push_back(p);
        }
    }

    // Left wall - 3 layers from left edge going right
    for (int layer = 0; layer < numLayers; layer++) {
        float x = margin + particleSpacing * layer;

        for (float y = 0.0f; y <= containerHeight - margin; y += particleSpacing) {
            Particle2D* p = new Particle2D(x, y, particleMass, particleRadius);
            p->isBoundary = true;
            p->velocity = Vec2(0.0f, 0.0f);
            p->color = 0xFF888888;
            particles.push_back(p);
        }
    }

    // Right wall - 3 layers from right edge going left
    for (int layer = 0; layer < numLayers; layer++) {
        float x = containerWidth - margin - particleSpacing * layer;

        for (float y = 0.0f; y <= containerHeight - margin; y += particleSpacing) {
            Particle2D* p = new Particle2D(x, y, particleMass, particleRadius);
            p->isBoundary = true;
            p->velocity = Vec2(0.0f, 0.0f);
            p->color = 0xFF888888;
            particles.push_back(p);
        }
    }
}
