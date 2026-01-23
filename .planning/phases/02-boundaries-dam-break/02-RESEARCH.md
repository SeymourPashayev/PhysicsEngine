# Phase 2: Boundaries & Dam Break - Research

**Researched:** 2026-01-23
**Domain:** SPH boundary conditions and solid wall treatment for WCSPH simulations
**Confidence:** MEDIUM

## Summary

SPH boundary conditions are "one of the most difficult technical points of the SPH method" due to kernel truncation at boundaries and particle deficiency issues. For this phase implementing rectangular container boundaries and a dam break scenario, the research identifies **Dynamic Boundary Conditions (DBC)** as the standard approach for WCSPH simulations.

DBC represents solid walls using fixed boundary particles that participate in density and pressure calculations but remain stationary. When fluid particles approach within 2h (twice the smoothing length) of boundary particles, the boundary particle density increases, creating repulsive pressure forces that prevent fluid penetration. This method is the default in DualSPHysics and provides a good balance between simplicity, robustness, and accuracy for rectangular geometries.

For the dam break scenario, standard practice initializes fluid particles in a regular grid at rest (zero velocity) on the left side of the container, with boundary particles forming the container walls at the same spacing as fluid particles. The simulation demonstrates realistic wave propagation, splashing, and energy dissipation when the fluid column collapses.

**Primary recommendation:** Implement Dynamic Boundary Conditions (DBC) with fixed boundary particles at the same spacing as fluid particles, participating in SPH density/pressure calculations but remaining stationary.

## Standard Stack

### Core
| Library | Version | Purpose | Why Standard |
|---------|---------|---------|--------------|
| DualSPHysics | 5.x+ | Reference implementation | Industry standard open-source WCSPH code, extensively validated |
| SPHinXsys | Latest | Alternative reference | Modern C++ SPH library with good documentation |

### Supporting
| Component | Purpose | When to Use |
|-----------|---------|-------------|
| Octree2D | Neighbor search | Already in codebase - reuse for boundary particles |
| Grid initialization | Particle placement | Setting up regular arrays of boundary and fluid particles |

### Alternatives Considered
| Instead of | Could Use | Tradeoff |
|------------|-----------|----------|
| DBC | Repulsive forces | Simpler but adds non-physical forces, worse accuracy |
| DBC | Ghost particles (mDBC) | Better accuracy but 25% more computation, complex corners |
| DBC | Simple reflection | Much simpler but causes leakage, poor for fluids |

**Implementation Notes:**
This phase uses existing SPHSolver and Particle2D classes. No new libraries needed - just extend existing architecture.

## Architecture Patterns

### Recommended Project Structure
```
src/
├── Physics/
│   ├── Particle2D.hpp/cpp          # Existing - add boundary flag
│   ├── SPHSolver.hpp/cpp           # Existing - modify neighbor search
│   ├── SPHKernel.hpp/cpp           # Existing - no changes
│   └── BoundaryParticles.hpp/cpp   # New - boundary initialization
├── Scenes/
│   └── DamBreak.hpp/cpp            # New - dam break scene
└── System.hpp/cpp                  # Existing - integrate boundary particles
```

### Pattern 1: Dynamic Boundary Conditions (DBC)

**What:** Fixed particles that participate in SPH calculations but don't move

**When to use:** Standard approach for WCSPH with solid walls

**How it works:**
1. Boundary particles placed at same spacing as fluid particles
2. Boundary particles included in neighbor search for fluid particles
3. Boundary particles contribute to fluid density summation: `ρ_fluid += m_boundary * W(r)`
4. Boundary particle pressure computed from density via Tait EOS (same as fluid)
5. Boundary particles contribute to fluid pressure forces
6. Boundary particle positions remain fixed (no integration)

**Example structure:**
```cpp
// Boundary particle properties
struct Particle2D {
    Vec2 position;
    Vec2 velocity;        // Zero for boundary particles
    float mass;           // Same as fluid particles
    float density;        // Computed from neighbors
    float pressure;       // Computed from Tait EOS
    bool isBoundary;      // NEW: Flag to identify boundary particles
    // ... existing SPH properties
};

// In SPHSolver::ComputeDensityPressure
for (auto& particle : particles) {
    std::vector<Particle2D*> neighbors;
    octree->findNeighbors(neighbors, particle, smoothingLength);

    float density = 0.0f;
    for (auto& neighbor : neighbors) {
        // Boundary particles contribute to fluid density
        // Fluid particles contribute to boundary density
        density += neighbor->mass * SPHKernel::Poly6(r);
    }
    particle->density = density;

    // Compute pressure for ALL particles (fluid and boundary)
    particle->pressure = ComputeTaitPressure(density);
}

// In integration step
for (auto& particle : particles) {
    if (!particle->isBoundary) {
        particle->SymplecticEulerIntegrate(dt);
    }
    // Boundary particles skip integration
}
```

### Pattern 2: Regular Grid Initialization

**What:** Place particles in uniform grid for both boundaries and fluid

**When to use:** Initial setup for rectangular containers and fluid regions

**Example:**
```cpp
// Boundary particles - rectangular container
void CreateContainerBoundaries(float width, float height, float spacing) {
    // Bottom wall
    for (float x = 0; x <= width; x += spacing) {
        for (int layer = 0; layer < 3; layer++) {  // Multi-layer for robustness
            Vec2 pos(x, layer * spacing);
            AddBoundaryParticle(pos);
        }
    }

    // Left wall
    for (float y = 0; y <= height; y += spacing) {
        for (int layer = 0; layer < 3; layer++) {
            Vec2 pos(layer * spacing, y);
            AddBoundaryParticle(pos);
        }
    }

    // Right wall (mirror left)
    // Top wall (if needed)
}

// Fluid particles - dam break configuration
void CreateDamBreakFluid(float columnWidth, float columnHeight, float spacing) {
    for (float x = 0; x < columnWidth; x += spacing) {
        for (float y = 0; y < columnHeight; y += spacing) {
            Vec2 pos(x, y);
            AddFluidParticle(pos);
        }
    }
}
```

### Pattern 3: Scene-Based Architecture

**What:** Separate scene classes for different simulation setups

**When to use:** Supporting multiple simulation scenarios (existing pattern in codebase)

**Structure:**
```cpp
class DamBreakScene : public Scene {
public:
    void Setup() override {
        // Create boundary particles
        CreateContainerBoundaries(containerWidth, containerHeight, particleSpacing);

        // Create fluid column on left side
        CreateDamBreakFluid(damWidth, damHeight, particleSpacing);

        // Initialize SPH solver
        sphSolver->Initialize(smoothingLength);
    }

    void Update(float dt) override {
        // SPH computations (density, pressure, forces)
        sphSolver->ComputeDensityPressure(particles, octree);
        sphSolver->ComputeForces(particles, octree);

        // Integrate only fluid particles
        for (auto* p : particles) {
            if (!p->isBoundary) {
                p->SymplecticEulerIntegrate(dt);
            }
        }
    }
};
```

### Anti-Patterns to Avoid

- **Screen collision instead of boundary particles:** Existing CheckForScreenCollisions uses velocity reflection - this is NOT suitable for SPH fluids. Fluid particles need to interact with boundary particles through pressure forces, not position clamping.

- **Zero-mass boundary particles:** Boundary particles must have same mass as fluid particles to contribute correctly to density summation.

- **Excluding boundaries from Octree:** Boundary particles must be in spatial data structure for efficient neighbor search.

- **Single-layer boundaries:** Use 2-3 layers of boundary particles to prevent leakage at high velocities or large time steps.

## Don't Hand-Roll

| Problem | Don't Build | Use Instead | Why |
|---------|-------------|-------------|-----|
| Boundary treatment | Custom repulsive forces | Dynamic Boundary Conditions (DBC) | DBC is validated standard, repulsive forces add non-physical energy |
| Corner handling | Special case detection | Standard DBC with multi-layer boundaries | Multi-layer boundaries naturally handle corners, no special logic needed |
| Particle initialization | Random placement with relaxation | Regular grid placement | Grid initialization is simpler, deterministic, and standard for benchmarks |
| Hydrostatic test | Visual inspection only | Quantitative pressure verification | Correct boundary treatment shows linear pressure gradient (ρgh), easy to verify programmatically |

**Key insight:** SPH boundary conditions have been extensively researched since the 1990s. The DBC method (fixed particles participating in SPH) is the converged solution for WCSPH - don't try to invent alternative approaches. Implementation is straightforward: add boundary flag, skip integration for boundary particles.

## Common Pitfalls

### Pitfall 1: Particle Deficiency at Boundaries

**What goes wrong:** Fluid particles near walls have incomplete kernel support (missing neighbors on the boundary side), leading to underestimated density and incorrect pressure.

**Why it happens:** SPH kernels assume spherical support region, but walls truncate this region.

**How to avoid:**
- Use Dynamic Boundary Conditions - boundary particles fill the kernel support
- Ensure boundary particles participate in density summation
- Use multi-layer boundaries (2-3 layers) for better kernel coverage

**Warning signs:**
- Particles "sticking" to walls
- Unphysical gaps between fluid and boundaries
- Fluid particles penetrating walls at corners
- Pressure at boundaries doesn't match hydrostatic distribution (P = ρgh)

### Pitfall 2: Unphysical Gap Between Fluid and Boundary

**What goes wrong:** Small gap forms between fluid and boundary particles, visible as fluid not quite touching walls. Pressure measurements at boundary are inaccurate.

**Why it happens:** Standard DBC can create slight repulsion due to how boundary density is computed.

**How to avoid:**
- Accept small gap as limitation of basic DBC (< 0.5 * particle spacing)
- Use same particle spacing for fluid and boundary
- Verify gap doesn't grow over time (sign of instability)
- For higher accuracy, consider Modified DBC (mDBC) in future phase

**Warning signs:**
- Gap larger than half a particle spacing
- Gap grows over time
- Hydrostatic test shows pressure error > 5% at boundaries

### Pitfall 3: Boundary Particles Moving During Simulation

**What goes wrong:** Boundary particles drift or vibrate, destroying solid wall.

**Why it happens:** Forgetting to skip integration for boundary particles, or accidentally applying forces.

**How to avoid:**
```cpp
// WRONG - integrates all particles
for (auto* p : particles) {
    p->SymplecticEulerIntegrate(dt);
}

// CORRECT - skip boundary particles
for (auto* p : particles) {
    if (!p->isBoundary) {
        p->SymplecticEulerIntegrate(dt);
    }
}

// Also skip force accumulation for boundaries
if (!particle->isBoundary) {
    particle->AddForce(gravityForce);
}
```

**Warning signs:**
- Boundary particles changing position
- Container "collapsing" or deforming
- Fluid escaping through boundary particles that moved

### Pitfall 4: Wrong Particle Spacing Ratio

**What goes wrong:** Using different spacing for boundary vs fluid particles causes density discontinuities.

**Why it happens:** Trying to "optimize" by using coarser boundary particles.

**How to avoid:**
- **Use same spacing for boundary and fluid particles**
- If spacing must differ, boundary spacing should be FINER than fluid (factor of 2), not coarser
- Standard practice: identical spacing

**Warning signs:**
- Density oscillations near boundaries
- Pressure spikes when fluid contacts walls
- Numerical instability in first few time steps

### Pitfall 5: Forgetting Boundary Particles in Octree Rebuild

**What goes wrong:** Only fluid particles inserted into Octree, boundary particles not found as neighbors.

**Why it happens:** Treating boundaries as separate from particle array.

**How to avoid:**
- Include all particles (fluid and boundary) in single particle array
- Rebuild Octree with complete particle set each timestep
- Use boundary flag to distinguish types, not separate arrays

**Warning signs:**
- Fluid particles penetrate walls immediately
- Zero pressure forces near boundaries
- Density at walls equals single particle mass (no neighbors found)

### Pitfall 6: Incorrect Boundary Particle Mass

**What goes wrong:** Setting boundary mass to zero or very large values causes incorrect density computation.

**Why it happens:** Misunderstanding that boundary particles must participate in density summation with physical mass.

**How to avoid:**
- **Boundary particles have same mass as fluid particles**
- Mass computed from: `m = ρ₀ * spacing²` (2D) or `m = ρ₀ * spacing³` (3D)
- Both fluid and boundary use same rest density

**Warning signs:**
- Extremely high or low density near boundaries
- Pressure at boundaries doesn't scale with depth
- Fluid either repelled too strongly or not repelled at all

## Code Examples

### Dynamic Boundary Condition Implementation

```cpp
// Source: Based on DualSPHysics DBC formulation
// https://github.com/DualSPHysics/DualSPHysics/wiki/3.-SPH-formulation

// Add boundary flag to Particle2D
struct Particle2D {
    // ... existing fields
    bool isBoundary = false;  // NEW: distinguish boundary particles
};

// Modified SPHSolver to handle boundaries
void SPHSolver::ComputeDensityPressure(std::vector<Particle2D*>& particles,
                                       Octree2D* octree) {
    for (auto* particle : particles) {
        std::vector<Particle2D*> neighbors;
        octree->findNeighbors(neighbors, particle, smoothingLength);

        // Compute density - includes BOTH fluid and boundary neighbors
        float density = 0.0f;
        for (auto* neighbor : neighbors) {
            Vec2 r = particle->position - neighbor->position;
            float W = SPHKernel::Poly6(r);
            density += neighbor->mass * W;
        }
        particle->density = density;

        // Compute pressure using Tait EOS - for ALL particles
        float densityRatio = density / restDensity;
        float pressure = stiffness * (std::pow(densityRatio, gamma) - 1.0f);
        pressure = std::max(0.0f, pressure);  // Clamp negative pressure
        particle->pressure = pressure;
    }
}

// Modified integration to skip boundary particles
void System::Advect(float dt, Vec2 pushForce) {
    for (auto* particle : particles) {
        // Only integrate fluid particles
        if (!particle->isBoundary) {
            particle->SymplecticEulerIntegrate(dt);
        }
        // Boundary particles remain fixed at their initial positions
    }
}
```

### Container Boundary Initialization

```cpp
// Source: Common practice from SPH dam break benchmarks
// https://www.sphinxsys.org/html/examples/example1_2D_dambreak.html

void CreateRectangularContainer(std::vector<Particle2D*>& particles,
                                float containerWidth,
                                float containerHeight,
                                float particleSpacing,
                                float particleMass) {
    int numLayers = 3;  // Multi-layer boundary for robustness

    // Bottom wall
    for (float x = -numLayers * particleSpacing;
         x <= containerWidth + numLayers * particleSpacing;
         x += particleSpacing) {
        for (int layer = 0; layer < numLayers; layer++) {
            Vec2 pos(x, -layer * particleSpacing);
            Particle2D* p = new Particle2D(pos, particleMass);
            p->isBoundary = true;
            p->velocity = Vec2(0.0f, 0.0f);
            particles.push_back(p);
        }
    }

    // Left wall
    for (float y = 0; y <= containerHeight; y += particleSpacing) {
        for (int layer = 0; layer < numLayers; layer++) {
            Vec2 pos(-layer * particleSpacing, y);
            Particle2D* p = new Particle2D(pos, particleMass);
            p->isBoundary = true;
            p->velocity = Vec2(0.0f, 0.0f);
            particles.push_back(p);
        }
    }

    // Right wall
    for (float y = 0; y <= containerHeight; y += particleSpacing) {
        for (int layer = 0; layer < numLayers; layer++) {
            Vec2 pos(containerWidth + layer * particleSpacing, y);
            Particle2D* p = new Particle2D(pos, particleMass);
            p->isBoundary = true;
            p->velocity = Vec2(0.0f, 0.0f);
            particles.push_back(p);
        }
    }

    // Note: Top is open for dam break scenario
}
```

### Dam Break Fluid Initialization

```cpp
// Source: Standard dam break benchmark setup
// https://link.springer.com/article/10.1007/s10652-025-10027-3

void CreateDamBreakFluid(std::vector<Particle2D*>& particles,
                         float columnWidth,
                         float columnHeight,
                         float particleSpacing,
                         float particleMass,
                         Vec2 offset = Vec2(0.0f, 0.0f)) {
    for (float x = offset.x; x < offset.x + columnWidth; x += particleSpacing) {
        for (float y = offset.y; y < offset.y + columnHeight; y += particleSpacing) {
            Vec2 pos(x, y);
            Particle2D* p = new Particle2D(pos, particleMass);
            p->isBoundary = false;  // Fluid particle
            p->velocity = Vec2(0.0f, 0.0f);  // Initial rest state
            particles.push_back(p);
        }
    }
}

// Typical usage for dam break on left side
void SetupDamBreak() {
    float spacing = 4.0f;  // Match existing particle size
    float containerWidth = Graphics::Width();
    float containerHeight = Graphics::Height();

    // Create container boundaries
    CreateRectangularContainer(particles, containerWidth, containerHeight,
                               spacing, particleMass);

    // Create fluid column on left side (1/4 width, 2/3 height)
    float damWidth = containerWidth * 0.25f;
    float damHeight = containerHeight * 0.66f;
    Vec2 fluidOffset(3 * spacing, 0.0f);  // Small offset from left wall
    CreateDamBreakFluid(particles, damWidth, damHeight, spacing,
                        particleMass, fluidOffset);
}
```

### Hydrostatic Pressure Verification

```cpp
// Source: Standard verification test for boundary conditions
// Clone particles paper: https://www.sciencedirect.com/science/article/pii/S0045782523000968

bool VerifyHydrostaticPressure(std::vector<Particle2D*>& particles,
                               float restDensity,
                               float gravity,
                               float tolerance = 0.05f) {
    // After simulation settles (fluid at rest), check pressure distribution
    // Expected: P = ρ₀ * g * (h_surface - y)

    float maxError = 0.0f;
    for (auto* p : particles) {
        if (!p->isBoundary) {
            // Find surface height (max y of fluid)
            float surfaceHeight = FindSurfaceHeight(particles);

            // Expected hydrostatic pressure
            float depth = surfaceHeight - p->position.y;
            float expectedPressure = restDensity * gravity * depth;

            // Actual pressure from simulation
            float actualPressure = p->pressure;

            // Compute relative error
            float error = std::abs(actualPressure - expectedPressure) /
                         (expectedPressure + 1e-6f);
            maxError = std::max(maxError, error);
        }
    }

    std::cout << "Hydrostatic test max error: " << maxError * 100 << "%" << std::endl;
    return maxError < tolerance;  // Should be < 5% for good boundary treatment
}
```

## State of the Art

| Old Approach | Current Approach | When Changed | Impact |
|--------------|------------------|--------------|--------|
| Repulsive forces at walls | Dynamic Boundary Conditions | ~2007 (Crespo et al.) | Physical pressure-based repulsion, no artificial forces |
| Single-layer boundaries | Multi-layer boundaries | ~2010s | Better kernel coverage, prevents high-velocity penetration |
| Simple reflection | DBC with pressure computation | ~2007 | Proper fluid-structure interaction, hydrostatic equilibrium |
| Separate boundary treatment | Unified particle framework | ~2010s | Simpler code, boundaries = particles with fixed positions |
| Modified DBC (ghost particles) | Standard DBC | 2021 (English et al.) | 25% overhead reduction while maintaining accuracy for simple geometries |

**Deprecated/outdated:**
- **Repulsive forces**: Adds non-physical energy, poor accuracy, deprecated in modern SPH codes
- **Velocity reflection at screen bounds**: Not suitable for SPH fluids, use boundary particles instead
- **Lennard-Jones potentials**: Historical approach, replaced by pressure-based DBC
- **Penalty forces**: Still used in some codes but DBC is more physically motivated

**Recent innovations (2024-2025):**
- **Improved single-layer boundaries**: Recent work shows single-layer can work with proper anti-penetration algorithms
- **Energy-conserving boundaries**: New research focuses on exact energy conservation (low priority for visual simulation)
- **GPU-optimized DBC**: Efficient implementations for massive particle counts

## Open Questions

### 1. Boundary Particle Color/Rendering
**What we know:** Boundary particles should be visually distinct from fluid
**What's unclear:** Should boundaries be rendered at all, or just container outline?
**Recommendation:** Render boundary particles in different color (gray/black) to verify placement, option to hide them and draw container outline instead

### 2. Existing Screen Collision Code
**What we know:** System::CheckForScreenCollisions() exists and uses velocity reflection
**What's unclear:** Should this be disabled when using boundary particles, or kept as fallback?
**Recommendation:** Disable SCREEN_COLLISION_ENABLED when using SPH boundary particles - they are mutually exclusive approaches. Keep code for non-SPH simulations.

### 3. Octree Rebuild Performance
**What we know:** Boundary particles are static, fluid particles move
**What's unclear:** Can we optimize by keeping boundary particles in separate static structure?
**Recommendation:** Start simple - rebuild full Octree every frame. Optimize only if profiling shows this is bottleneck. Premature optimization risks bugs.

### 4. Smoothing Length for Boundaries
**What we know:** Standard practice uses same h for fluid and boundary
**What's unclear:** Would adaptive smoothing length help at boundaries?
**Recommendation:** Use same h for all particles. Adaptive smoothing is complex optimization for future phases.

## Sources

### Primary (HIGH confidence)
- [DualSPHysics Wiki - SPH Formulation](https://github.com/DualSPHysics/DualSPHysics/wiki/3.-SPH-formulation) - Official documentation of DBC implementation
- [Modified dynamic boundary conditions (mDBC)](https://link.springer.com/article/10.1007/s40571-021-00403-3) - Peer-reviewed paper on mDBC method
- [SPHinXsys 2D Dam Break Example](https://www.sphinxsys.org/html/examples/example1_2D_dambreak.html) - Reference implementation with exact parameters
- [Lucas Schuermann - Implementing SPH in 2D](https://lucasschuermann.com/writing/implementing-sph-in-2d) - Tutorial with code examples

### Secondary (MEDIUM confidence)
- [Smoothed particle hydrodynamics (SPH) investigation of dam break flows](https://link.springer.com/article/10.1007/s10652-025-10027-3) - Recent 2025 research paper
- [Novel density correction method for improved pressure fields](https://onlinelibrary.wiley.com/doi/10.1002/nme.70116) - 2025 paper on pressure accuracy
- [A generalized wall boundary condition for SPH](https://www.sciencedirect.com/science/article/abs/pii/S002199911200229X) - Academic research on boundary methods
- [SPH particle boundary forces for arbitrary boundaries](https://www.sciencedirect.com/science/article/abs/pii/S0010465509001544) - Alternative boundary force methods
- [Clone particles: Simplified boundary conditions](https://www.sciencedirect.com/science/article/pii/S0045782523000968) - Recent simplified approach

### Tertiary (LOW confidence - general background)
- [Wikipedia - Smoothed-particle hydrodynamics](https://en.wikipedia.org/wiki/Smoothed-particle_hydrodynamics) - General overview
- [Algorithms for uniform particle initialization](https://arxiv.org/pdf/1910.07898) - Particle packing methods
- WebSearch results on SPH boundary treatment (various years) - Community discussions and blog posts

## Metadata

**Confidence breakdown:**
- Standard stack: HIGH - DBC is well-established standard in DualSPHysics and academic literature
- Architecture: HIGH - Pattern is straightforward extension of existing SPHSolver architecture
- Pitfalls: MEDIUM - Based on research papers and common implementation issues, not hands-on experience

**Research date:** 2026-01-23
**Valid until:** ~90 days (SPH boundary methods are mature, slow-changing field)

**Key uncertainties:**
1. Exact rendering approach for boundaries (low impact - aesthetic choice)
2. Interaction with existing screen collision code (medium impact - requires code review)
3. Performance optimization strategies (low impact - start simple, optimize later)

**Verification strategy:**
- Hydrostatic test: Pressure should match ρgh with < 5% error
- Visual test: No penetration at walls or corners during dam break
- Conservation test: Total mass should remain constant (sum of all particle masses)
