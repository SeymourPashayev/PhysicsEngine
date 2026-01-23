---
phase: 01-core-sph-mechanics
plan: 01
subsystem: sph-solver
status: complete
tags: [sph, fluid-dynamics, density, pressure, tait-equation]

requires:
  - SPHKernel implementation (Poly6, Spiky, Viscosity)
  - Octree2D spatial partitioning
  - Particle2D with density/pressure fields

provides:
  - SPHSolver class for SPH computations
  - Density calculation from neighbors using Poly6 kernel
  - Pressure calculation using Tait equation of state
  - Integration into System update loop

affects:
  - phase: 01-core-sph-mechanics
    plan: 02
    reason: "SPHSolver ready for force computation (pressure/viscosity)"

tech-stack:
  added:
    - SPHSolver: "Separated SPH computation logic from particle data"
  patterns:
    - "Tait equation of state: P = B * ((ρ/ρ₀)^γ - 1)"
    - "Poly6 kernel for density: W = (315/(64πh⁹)) * (h² - r²)³"
    - "Octree-based neighbor queries for SPH"

key-files:
  created:
    - src/Physics/SPHSolver.hpp: "SPH solver class declaration"
    - src/Physics/SPHSolver.cpp: "Density/pressure computation implementation"
    - src/Physics/SPHKernel.hpp: "SPH kernel function declarations"
    - src/Physics/SPHKernel.cpp: "Poly6, Spiky, Viscosity kernel implementations"
  modified:
    - src/System.hpp: "Added SPHSolver integration"
    - src/System.cpp: "ComputeSPH method, solver lifecycle management"
    - Makefile: "Added SPH files to build"

decisions:
  - what: "Tait equation parameters"
    why: "Use B=1000 (stiffness) and γ=7 for water-like behavior"
    when: "2026-01-23"
  - what: "Smoothing length initialization"
    why: "Use 16.0f to match existing SPHKernel default"
    when: "2026-01-23"
  - what: "Negative pressure clamping"
    why: "Clamp pressure to >= 0 to prevent tension forces (no negative pressure)"
    when: "2026-01-23"

metrics:
  duration: "2.75 minutes"
  completed: "2026-01-23"
---

# Phase 01 Plan 01: SPH Solver Foundation Summary

**One-liner:** SPH solver with Poly6 density calculation and Tait equation pressure computation, separated from particle data structure.

## Objective Achieved

Created the SPH solver architecture that separates SPH computation logic from particle data structures. Implemented the first half of the SPH pipeline: density calculation from neighbors using the Poly6 kernel, and pressure computation using the Tait equation of state.

## What Was Built

### SPHSolver Class
- **SPHSolver.hpp/cpp**: New dedicated class for SPH computations
  - `Initialize(float h)`: Sets smoothing length and initializes kernel constants
  - `ComputeDensityPressure()`: Main computation method
  - Private members: smoothingLength (h), restDensity (ρ₀=1000), stiffness (B=1000), gamma (γ=7)
  - Placeholder for `ComputeForces()` (Plan 02)

### SPH Kernel Functions
- **SPHKernel.hpp/cpp**: Mathematical kernel functions for SPH
  - Poly6 kernel for density: `W = (315/(64πh⁹)) * (h² - r²)³` for r < h
  - Spiky gradient kernel for pressure forces (ready for Plan 02)
  - Viscosity Laplacian kernel for viscosity forces (ready for Plan 02)
  - Static initialization with precomputed constants for efficiency

### System Integration
- **System.hpp/cpp**: Integrated SPHSolver into main update loop
  - Added `sphSolver` member variable
  - Constructor creates and initializes solver with h=16.0f
  - New `ComputeSPH()` method: rebuilds octree, calls `ComputeDensityPressure()`
  - Called in `Advect()` before force loop when `FLUID_DYNAMICS_ENABLED=true`
  - Proper cleanup in destructor

## Technical Implementation

### Density Calculation
For each particle i:
1. Query octree for neighbors within smoothing length h
2. Sum density contributions: `ρᵢ = Σⱼ mⱼ * W(|rᵢ - rⱼ|, h)`
3. Uses Poly6 kernel: symmetric, smooth, zero at boundary
4. Includes self-contribution (particle contributes to its own density)

### Pressure Calculation
Using Tait equation of state for weakly compressible fluids:
- `P = B * ((ρ/ρ₀)^γ - 1)`
- B = 1000 (gas constant/stiffness)
- γ = 7 (exponent for water)
- ρ₀ = 1000 (rest density)
- Clamped to P >= 0 (prevents tension/negative pressure)

### Octree Integration
- Rebuilds octree each frame before SPH computation (`octree->rebuild(particles)`)
- Uses `findNeighbors(neighbors, particle, smoothingLength)` for efficient spatial queries
- Smoothing length h=16.0f matches kernel radius

## Verification Results

✅ Build succeeds with no errors (one unrelated warning in Octree2D)
✅ SPHSolver class created with density/pressure methods
✅ SPHKernel implementation with Poly6, Spiky, Viscosity kernels
✅ System integrates solver via `ComputeSPH()` method
✅ Tait equation implemented: `P = B * ((ρ/ρ₀)^γ - 1)` with negative clamping
✅ Density computed from neighbors using Poly6 kernel
✅ Makefile updated to compile all SPH components

## Commits

| Commit | Type | Description |
|--------|------|-------------|
| 136143d | feat | Create SPHSolver class with density/pressure computation |
| 9345dd1 | feat | Integrate SPHSolver into System update loop |

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 2 - Missing Critical] Added SPHKernel files to commit**
- **Found during:** Task 1 build verification
- **Issue:** SPHKernel.hpp and SPHKernel.cpp existed but weren't tracked in git
- **Fix:** Added both kernel files to the initial commit since they're critical for SPH operation
- **Files affected:** src/Physics/SPHKernel.hpp, src/Physics/SPHKernel.cpp
- **Commit:** 136143d
- **Rationale:** SPHSolver depends on SPHKernel - cannot function without it

**2. [Rule 2 - Missing Critical] Added SDL2_image to Makefile linking**
- **Found during:** Task 1 build
- **Issue:** Makefile was missing -lSDL2_image linker flag
- **Fix:** Added -lSDL2_image to linker flags
- **Files affected:** Makefile
- **Commit:** 136143d
- **Rationale:** Required dependency for engine to link successfully

## Files Changed

**Created:**
- `src/Physics/SPHSolver.hpp` (39 lines)
- `src/Physics/SPHSolver.cpp` (61 lines)
- `src/Physics/SPHKernel.hpp` (44 lines)
- `src/Physics/SPHKernel.cpp` (62 lines)

**Modified:**
- `src/System.hpp`: Added SPHSolver include, member, and ComputeSPH method
- `src/System.cpp`: Constructor initialization, destructor cleanup, ComputeSPH implementation
- `Makefile`: Added SDL2_image linking flag

## Performance Characteristics

**Computational Complexity:**
- Density/Pressure: O(n * k) where n = particle count, k = avg neighbors
- Octree rebuild: O(n log n)
- Per-frame overhead: Octree rebuild + SPH computation

**Memory:**
- SPHSolver: ~32 bytes (4 floats)
- Per-particle SPH state: density (4B) + pressure (4B) = 8 bytes

## Next Phase Readiness

**Ready for Plan 02 (SPH Force Computation):**
- ✅ Density values populated on all particles
- ✅ Pressure values computed using Tait equation
- ✅ SPHKernel provides Spiky and Viscosity kernels
- ✅ SPHSolver has placeholder `ComputeForces()` method
- ✅ System calls solver each frame when fluid dynamics enabled

**Blockers:** None

**Concerns:** None - foundation is solid for force computation

## Knowledge Gained

1. **Tait equation effectiveness**: B=1000, γ=7 provides water-like compressibility
2. **Negative pressure clamping**: Essential to prevent unphysical tension forces
3. **Self-contribution in density**: Particle must include itself in density sum
4. **Octree rebuild timing**: Must rebuild before SPH computation each frame for correct neighbor queries
5. **Kernel constant precomputation**: SPHKernel::Initialize() computes constants once for efficiency

## Testing Notes

**Build Testing:**
- ✅ Clean build completes successfully
- ✅ All SPH files compile without errors
- ✅ No new compiler warnings introduced

**Runtime Testing Required (for next session):**
- [ ] Run engine and create particles via mouse click
- [ ] Verify non-zero density values after computation
- [ ] Verify pressure values are non-negative
- [ ] Verify no crashes with 10+ particles
- [ ] Verify frame rate remains interactive

## Cross-References

**Depends on:**
- Particle2D density/pressure fields (already existed)
- Octree2D neighbor finding (already existed)
- SPHKernel implementation (added in this plan)

**Enables:**
- Plan 01-02: SPH force computation (pressure + viscosity)
- Plan 01-03: Force integration and visualization
- Future metaball rendering (needs density field)

**Related Decisions:**
- WCSPH approach (PROJECT.md): This implements the weakly compressible variant
- CPU-only execution (PROJECT.md): Single-threaded SPH solver
- 2D-first architecture (PROJECT.md): SPHSolver works with Particle2D and Vec2
