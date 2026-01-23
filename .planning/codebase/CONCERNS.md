# Codebase Concerns

**Analysis Date:** 2026-01-23

## Tech Debt

**Hardcoded Physics Parameters:**
- Issue: Physics constants and magic numbers scattered throughout codebase instead of centralized configuration
- Files: `src/System.cpp:140` (hardcoded collision bouncing -0.9f), `src/Scenes/WaterDam.cpp:31`, `src/Scenes/SpringForceLattice.cpp:28,60,66`
- Impact: Difficult to tune physics behavior; requires code recompilation to change simulation parameters; inconsistent values across different scenes
- Fix approach: Create configuration files or command-line arguments for runtime physics tuning; centralize all magic numbers in `Constants.hpp`

**Incomplete SPH Implementation:**
- Issue: SPH kernel functions implemented (`src/Physics/SPHKernel.cpp`) but not integrated into particle density/pressure calculations in `src/Physics/Particle2D.hpp`
- Files: `src/Physics/Particle2D.hpp:82-86` (SPH method declarations), `src/Physics/Particle2D.cpp:137-139` (ComputeDistanceField returns 0.0f stub)
- Impact: Fluid dynamics features advertised but non-functional; wasted computation on unused SPH kernels
- Fix approach: Complete implementation in `Particle2D::ComputeDensity()`, `ComputePressure()`, `ComputePressureForce()`, and `ComputeViscosityForce()` using the kernel functions

**TODO Comments and Unresolved Design Decisions:**
- Issue: Multiple TODO items indicating incomplete design decisions
- Files: `src/System.cpp:140` (FIXME hardcoded values), `src/Scenes/WaterDam.cpp:31` (TODO PIXELS_PER_METER conversion), `src/Scenes/SpringForceLattice.cpp:28,60,66` (TODO PIXELS_PER_METER), `src/Physics/Force.cpp:36` (TODO uncertain physics units)
- Impact: Inconsistent unit handling; code maintainer uncertainty about coordinate systems and scaling
- Fix approach: Document the coordinate system specification; complete all pixel-to-physics conversions; clarify why drag doesn't use PIXELS_PER_METER

**Unclear Unit Scaling:**
- Issue: Mixed usage of PIXELS_PER_METER across force calculations; drag force line 36 in `src/Physics/Force.cpp` has commented-out PIXELS_PER_METER with uncertain reasoning
- Files: `src/Physics/Force.cpp:36,55,107` (inconsistent PIXELS_PER_METER application), `src/Physics/Constants.hpp` (PIXELS_PER_METER definition)
- Impact: Potential physics simulation inaccuracy; forces may not scale correctly with different screen resolutions
- Fix approach: Establish unit convention documentation; audit all force calculations for consistent scaling; uncomment or justify the omission of PIXELS_PER_METER in drag

## Performance Bottlenecks

**O(n²) Particle Collision Detection:**
- Problem: `CheckForParticleCollisions()` iterates all particle pairs without spatial optimization
- Files: `src/System.cpp:167-177`
- Cause: Nested loop checking every particle against every other particle
- Improvement path: Use Octree neighbor finding (already available) to reduce search space; only check particles within each other's spatial partition

**Octree Rebuild Cost:**
- Problem: Octree must be rebuilt or particles inserted individually after each frame despite `rebuild()` method existing
- Files: `src/System.cpp:57-120` (no octree rebuild call), `src/Physics/Octree2D.hpp:42` (rebuild method exists but unused)
- Cause: Particles move every frame but octree is never cleared/rebuilt; particles accumulate in wrong spatial locations
- Improvement path: Call `octree->rebuild()` at start of each `Advect()` frame or implement dynamic insertion with bounds checking

**Octree Contains Unbounded Particles:**
- Problem: Particles can move outside octree bounds without triggering relocation
- Files: `src/Physics/Octree2D.cpp:107-124` (insert checks bounds but doesn't relocate)
- Cause: `insert()` silently fails for out-of-bounds particles; screen collision may move particles outside initial octree boundaries
- Improvement path: Implement dynamic bounds expansion or particle pruning; log warnings for out-of-bounds insertions

**Multiple Vector Iterations:**
- Problem: Force calculations iterate neighbors vector, then later particle list iterated again for collisions
- Files: `src/System.cpp:82-119` (forces), `src/System.cpp:167-177` (collisions)
- Cause: No opportunity for batching spatial queries
- Improvement path: Combine spatial query operations where possible; cache neighbor results across force types

## Fragile Areas

**Octree2D Design Fragility:**
- Files: `src/Physics/Octree2D.cpp:58-105`, `src/Physics/Octree2D.hpp:65-75`
- Why fragile:
  1. Fixed maximum depth (maxDepth = 6) doesn't adapt to particle density
  2. maxParticlesPerNode (16) is declared but never enforced in buildOctree logic
  3. Two different findNeighbors overloads with inconsistent boundary checking
  4. Children array manually managed with raw pointers and manual delete
  5. Thread safety members declared (`nodeMutex`) but never used
- Safe modification: Add assertions for spatial consistency; test with particles outside bounds; verify neighbor finding returns consistent results across overloads

**Particle State Management:**
- Files: `src/Physics/Particle2D.hpp:25-95`, `src/Physics/Particle2D.cpp`
- Why fragile:
  1. SPH properties initialized to zero but never computed during typical simulation
  2. Multiple integration methods (Euler, Verlet, RK4) but only RK4 used in system
  3. operator+ overload doesn't match C++ conventions (should return Vec2, not void)
  4. Distance() method const-correct but other collision methods not
- Safe modification: Clear which integration method is active; decide on SPH integration or remove SPH properties; fix operator overloading

**Memory Management with Raw Pointers:**
- Files: `src/System.cpp:13-53`, `src/System.hpp:33-42`, `src/Physics/Octree2D.cpp:47-56`
- Why fragile:
  1. Manual new/delete throughout for particles, springs, octree children
  2. No bounds checking before accessing particle arrays
  3. Particle vectors can contain null pointers (checked in draw)
  4. Octree children may be nullptr, requires defensive null checks throughout
- Safe modification: Consider using std::unique_ptr or std::vector exclusively; add bounds assertions

**Collision Resolution Inconsistency:**
- Files: `src/Physics/Particle2D.cpp:154-199`
- Why fragile:
  1. Screen collision applies -0.9f damping factor
  2. Particle collision uses coefficient of restitution (0.75f) but only as declaration, not applied
  3. Collision detection and resolution duplicate distance calculation
  4. Mixed use of Normalize() vs UnitVector() for direction vectors
- Safe modification: Unify collision response using consistent damping/restitution; eliminate duplicate calculations; ensure all collision types apply same physical laws

## Memory and Resource Issues

**Potential Memory Leak in Octree Children:**
- Issue: Octree recursively creates children with new but destructor deletes them; however, if Clear() is called but octree not destroyed, memory leaks
- Files: `src/Physics/Octree2D.cpp:231-240` (Clear), `src/Physics/Octree2D.hpp:27-32`
- Risk: Extended simulations with repeated Clear()/rebuild() may accumulate orphaned allocations
- Mitigation: Audit Clear() call patterns; verify RAII principles; test for leaks under reset scenarios

**Spring Vector Never Used:**
- Issue: Springs vector allocated and managed but SPRINGFORCE_ENABLED code path commented out
- Files: `src/System.cpp:42-50` (Spring deletion in destructor), `src/System.cpp:111-113` (SPRINGFORCE_ENABLED commented)
- Risk: Memory allocated for springs but never computed; wasted allocation if scenes attempt spring setup
- Mitigation: Either implement spring forces or remove spring storage entirely

## Security Considerations

**No Bounds Validation on Mouse Input:**
- Risk: Mouse position used directly without bounds checking to create particles
- Files: `src/System.cpp:181-202` (CreateParticleAtMouse), `src/Application.cpp:93-96` (SDL_MOUSEMOTION)
- Current mitigation: Screen collision runs after particle creation
- Recommendations: Validate mouse coordinates against screen bounds before particle creation; clamp to valid range

**No Protection Against Division by Zero:**
- Risk: Several distance calculations and normalizations could produce NaN
- Files: `src/Physics/Vec2.cpp` (UnitVector/Normalize), `src/Physics/SPHKernel.cpp:46` (distance == 0.0f check exists), `src/Physics/Particle2D.cpp:112-116`
- Current mitigation: Some checks exist for zero distance in kernel functions
- Recommendations: Add consistent zero-distance guards to all normalization operations; test with coincident particles

## Test Coverage Gaps

**No Tests Exist:**
- What's not tested: Physics equations, integration methods, collision detection, octree spatial partitioning, SPH kernel functions
- Files: No test directory found in repository
- Risk: Simulation inaccuracy undetected; refactoring breaks physics without notice; octree insertion bugs go unnoticed
- Priority: High - physics engine correctness is critical

**Unverified Octree Behavior with Dynamic Particles:**
- What's not tested: Particle insertion after octree creation; neighbor finding accuracy with moving particles; bounds handling
- Files: `src/Physics/Octree2D.cpp`
- Risk: Spatial queries may miss particles or include false neighbors; collision detection unreliable
- Priority: High - affects core simulation

**SPH Functions Never Exercised:**
- What's not tested: Kernel constant computation; SPH density/pressure calculations; viscosity application
- Files: `src/Physics/SPHKernel.cpp`, `src/Physics/Particle2D.hpp:82-86`
- Risk: SPH features broken or incorrect; code assumes to work but non-functional
- Priority: Medium - not currently used but advertised as feature

## Known Issues

**Octree Particle Lookup Fails for Moving Particles:**
- Symptoms: Neighbor finding may not locate particles that have moved outside their original octree cell
- Files: `src/Physics/Octree2D.cpp:107-124` (insert implementation), `src/System.cpp:57` (no rebuild)
- Trigger: Run simulation with high velocities for multiple frames
- Workaround: Manually call octree->rebuild() between frames (but currently not done)

**Screen Collision Hardcoded to Graphics Class:**
- Symptoms: Physics layer tightly coupled to Graphics; screen resolution changes require code modification
- Files: `src/System.cpp:138-162` uses `Graphics::Height()` and `Graphics::Width()`
- Trigger: Any attempt to resize window or change resolution
- Workaround: Inject window dimensions as parameters instead of using static Graphics references

**RK4 Integration Uses Forces But Doesn't Recalculate:**
- Symptoms: RK4 method uses intermediate velocity states but doesn't recalculate forces at intermediate steps
- Files: `src/Physics/Particle2D.cpp:84-108`
- Trigger: Particles accelerating non-uniformly show slightly incorrect trajectories
- Workaround: Not viable without force callback; current implementation is physically approximate not exact

---

*Concerns audit: 2026-01-23*
