// SPHSolver.cpp
// Created: Thu 23 Jan 2026
// ------------------------
// SPH Solver Implementation

#include "SPHSolver.hpp"
#include <cmath>

SPHSolver::SPHSolver()
    : smoothingLength(0.0f)
    , restDensity(REST_DENSITY)
    , stiffness(1000.0f)
    , gamma(7.0f)
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
        // P = B * ((ρ/ρ₀)^γ - 1)
        float densityRatio = density / restDensity;
        float pressure = stiffness * (std::pow(densityRatio, gamma) - 1.0f);

        // Clamp pressure to non-negative values (no tension)
        if (pressure < 0.0f) {
            pressure = 0.0f;
        }

        // Store pressure
        particle->pressure = pressure;
    }
}

void SPHSolver::ComputeForces(std::vector<Particle2D*>& particles, Octree2D* octree) {
    // Placeholder for Plan 02
    // Will implement pressure and viscosity force computation
}
