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

    float particleSpacing = 4.0f;  // Matches existing particle size
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

    // Start offset: 3 layers away from left wall to avoid overlap
    Vec2 offset(3.0f * particleSpacing, 0.0f);

    // Create regular grid of fluid particles
    for (float x = offset.x; x < offset.x + damWidth; x += particleSpacing) {
        for (float y = 0.0f; y < damHeight; y += particleSpacing) {

            // Create fluid particle
            Particle2D* p = new Particle2D(Vec2(x, y), particleMass);
            p->isBoundary = false;
            p->velocity = Vec2(0.0f, 0.0f);  // Zero initial velocity
            p->color = 0xFF4499FF;          // Blue fluid

            particles.push_back(p);
        }
    }

    // Update particle count
    particleCount = particles.size();

    std::cout << "DamBreak scene initialized:" << std::endl;
    std::cout << "  Total particles: " << particleCount << std::endl;
    std::cout << "  Fluid particles: " << (particleCount - particleCount) << std::endl;
    std::cout << "  Container: " << containerWidth << " x " << containerHeight << std::endl;
    std::cout << "  Dam column: " << damWidth << " x " << damHeight << std::endl;
}

DamBreak::~DamBreak() {
    // Base System destructor handles particle cleanup
}
