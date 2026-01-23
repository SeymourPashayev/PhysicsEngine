// SPHSolver.cpp
// Created: Thu 23 Jan 2026
// ------------------------
// SPH Solver Implementation

#include "SPHSolver.hpp"
#include <cmath>

SPHSolver::SPHSolver()
    : smoothingLength(0.0f)
    , restDensity(REST_DENSITY)
    , stiffness(50000.0f)  // High stiffness for boundary containment
    , gamma(7.0f)
    , viscosity(VISCOSITY)
{
}

void SPHSolver::Initialize(float smoothingLength) {
    this->smoothingLength = smoothingLength;
    SPHKernel::Initialize(smoothingLength);
}

void SPHSolver::ComputeDensityPressure(std::vector<Particle2D*>& particles, Octree2D* octree) {

    // Compute density and pressure for each particle
    for (auto& particle : particles) {
        // Find neighbors within smoothing length
        std::vector<Particle2D*> neighbors;
        octree->findNeighbors(neighbors, particle, smoothingLength);

        // Compute density: ρᵢ = Σⱼ mⱼ * W(rᵢⱼ, h)
        float density = 0.0f;

        for (auto& neighbor : neighbors) {
            Vec2 r = particle->position - neighbor->position;
            float kernelValue = SPHKernel::Poly6(r);
            density += neighbor->mass * kernelValue;
        }

        // Store density
        particle->density = density;

        // Compute pressure using Tait equation of state
        // P = B * ((ρ/ρ₀)^γ - 1) where γ=7
        // Use multiplication instead of pow() for speed: x^7 = x * x^2 * x^4
        float densityRatio = density / restDensity;
        float dr2 = densityRatio * densityRatio;
        float dr4 = dr2 * dr2;
        float dr7 = densityRatio * dr2 * dr4;
        float pressure = stiffness * (dr7 - 1.0f);

        // Clamp pressure to non-negative values (no tension)
        if (pressure < 0.0f) {
            pressure = 0.0f;
        }

        // Store pressure
        particle->pressure = pressure;
    }
}

void SPHSolver::ComputeForces(std::vector<Particle2D*>& particles, Octree2D* octree) {
    for (auto* particle : particles) {
        // Reset SPH forces
        particle->pressureForce = Vec2(0.0f, 0.0f);
        particle->viscosityForce = Vec2(0.0f, 0.0f);

        // Get neighbors
        std::vector<Particle2D*> neighbors;
        octree->findNeighbors(neighbors, particle, smoothingLength);

        for (auto* neighbor : neighbors) {
            if (neighbor == particle) continue;  // Skip self

            Vec2 r = particle->position - neighbor->position;
            float distance = r.Magnitude();

            if (distance < 0.0001f) continue;  // Skip nearly coincident particles

            // Pressure force (symmetric formulation)
            // F_pressure = -sum(m_j * (p_i + p_j) / (2 * rho_j) * grad_W)
            float pressureTerm = (particle->pressure + neighbor->pressure) / (2.0f * neighbor->density);
            Vec2 pressureGrad = SPHKernel::SpikyGradient(r);
            particle->pressureForce -= pressureGrad * (neighbor->mass * pressureTerm);

            // Viscosity force
            // F_viscosity = mu * sum(m_j * (v_j - v_i) / rho_j * laplacian_W)
            Vec2 velocityDiff = neighbor->velocity - particle->velocity;
            float viscosityLaplacian = SPHKernel::ViscosityLaplacian(distance);
            particle->viscosityForce += velocityDiff * (viscosity * neighbor->mass / neighbor->density * viscosityLaplacian);
        }
    }
}
