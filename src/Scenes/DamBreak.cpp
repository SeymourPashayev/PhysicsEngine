// DamBreak.cpp
// Created: Thu 23 Jan 2026
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Dam Break Simulation with SPH and Boundary Particles
//
// Classic dam break benchmark: tall fluid column on left side collapses
// under gravity, demonstrating SPH pressure forces, wave propagation,
// and boundary particle containment.

#include "DamBreak.hpp"
#include "../Physics/BoundaryParticles.hpp"
#include <algorithm>

DamBreak::DamBreak(Mouse* mouse) : System(mouse) {

    // ========================================
    // Configure Physics Toggles
    // ========================================

    FLUID_DYNAMICS_ENABLED = true;    // SPH pressure and viscosity
    INTEGRATION_ENABLED = true;       // Symplectic Euler integration
    GRAVITY_ENABLED = true;           // Gravity pulls fluid down
    SCREEN_COLLISION_ENABLED = false; // Using boundary particles instead

    // Disable other forces
    ATTRACTION_ENABLED = false;
    FRICTION_ENABLED = false;
    DRAG_ENABLED = false;
    PARTICLE_COLLISION_ENABLED = false;
    SPRINGFORCE_ENABLED = false;

    // Debug switches
    DEV_MENU_ENABLED = false;
    MOUSE_ENABLED = false;
    KEYBOARD_ENABLED = false;

    // ========================================
    // Calculate Parameters
    // ========================================

    float particleSpacing = 16.0f;  // Larger spacing for ~1500 particles (matches smoothing length)
    float particleMass = REST_DENSITY * particleSpacing * particleSpacing;

    float containerWidth = Graphics::Width();
    float containerHeight = Graphics::Height();

    // Dam dimensions: 1/4 width, 2/3 height (tall column on left)
    float damWidth = containerWidth * 0.25f;
    float damHeight = containerHeight * 0.66f;

    // ========================================
    // Create Boundary Particles (Container)
    // ========================================

    CreateRectangularContainer(
        particles,
        containerWidth,
        containerHeight,
        particleSpacing,
        particleMass
    );

    // Set boundary particle color (dark gray)
    for (Particle2D* p : particles) {
        if (p->isBoundary) {
            p->color = 0xFF333333;
        }
    }

    // ========================================
    // Create Fluid Column (Left Side)
    // ========================================

    // Start offset: close enough to left wall for SPH interaction
    // Left wall ends at margin + 2*spacing = 16 + 32 = 48
    // Smoothing length = 32 (2*spacing)
    // Fluid at x=64 has X-distance of 16 to wall, leaving room for Y-distance
    float margin = particleSpacing;
    Vec2 offset(margin + 3.0f * particleSpacing, 0.0f);  // x = 16 + 48 = 64

    // Create regular grid of fluid particles
    // In SDL: Y increases downward, so fluid at top (y=0) falls to bottom
    float particleRadius = particleSpacing / 2.0f;

    // Limit damHeight to stay within interaction range of bottom boundary
    // Bottom boundary topmost layer is at containerHeight - margin - 2*spacing = 1072
    // Fluid should end 1 spacing above that for proper SPH interaction
    float maxFluidY = containerHeight - margin - 3.0f * particleSpacing;  // = 1120 - 16 - 48 = 1056
    float actualDamHeight = std::min(damHeight, maxFluidY);

    for (float x = offset.x; x < offset.x + damWidth; x += particleSpacing) {
        for (float y = 0.0f; y < actualDamHeight; y += particleSpacing) {

            // Create fluid particle with proper radius
            Particle2D* p = new Particle2D(x, y, particleMass, particleRadius);
            p->isBoundary = false;
            p->velocity = Vec2(0.0f, 0.0f);  // Zero initial velocity
            p->color = 0xFF4499FF;          // Blue fluid

            particles.push_back(p);
        }
    }

    // Update particle count
    particleCount = particles.size();

    // Count boundary vs fluid particles
    int boundaryCount = 0;
    for (Particle2D* p : particles) {
        if (p->isBoundary) boundaryCount++;
    }

    std::cout << "DamBreak scene initialized:" << std::endl;
    std::cout << "  Total particles: " << particleCount << std::endl;
    std::cout << "  Boundary particles: " << boundaryCount << std::endl;
    std::cout << "  Fluid particles: " << (particleCount - boundaryCount) << std::endl;
    std::cout << "  Container: " << containerWidth << " x " << containerHeight << std::endl;
    std::cout << "  Dam column: " << damWidth << " x " << actualDamHeight << std::endl;
}

DamBreak::~DamBreak() {
    // Base System destructor handles particle cleanup
}
