# Project Research Summary

**Project:** WCSPH Fluid Simulation Integration
**Domain:** Real-time physics simulation (Smoothed Particle Hydrodynamics)
**Researched:** 2026-01-23
**Confidence:** HIGH

## Executive Summary

This project adds Weakly Compressible SPH (WCSPH) fluid simulation to an existing particle physics engine for real-time visual demonstration. The domain is well-established with clear best practices: modern implementations use Wendland C2 kernels for stability, symplectic Euler integration for efficiency, and density diffusion (delta-SPH) to prevent pressure noise. The target is 1,000-5,000 particles at 60fps on CPU, which is achievable with proper neighbor search and parameter tuning.

The recommended approach integrates SPH as a standalone solver component in the existing Physics layer, leveraging the current Octree2D for neighbor search and Particle2D for data storage. Critical success factors are: (1) implementing delta-SPH density diffusion to eliminate jitter, (2) using adaptive timesteps based on CFL conditions to prevent explosions, and (3) tuning the artificial speed of sound (20√(gH)) to balance stability and performance. The architecture cleanly separates particle data from SPH algorithms, enabling future 3D extension without rewrites.

Key risks are numerical stability (pressure oscillations, particle explosions) and performance bottlenecks (neighbor search consuming 80-90% of CPU time). Mitigation is well-documented: density diffusion prevents noise, CFL-based adaptive timestepping prevents instability, and spatial hashing or octree radius queries achieve O(n) neighbor search. The implementation roadmap follows natural dependencies: kernel functions → density/pressure → forces → boundaries → optimization.

## Key Findings

### Recommended Stack

The research converges on a specific WCSPH configuration optimized for real-time CPU simulation with visual quality targets. Wendland C2 kernels provide the best stability-performance tradeoff with compact support (zero beyond smoothing radius h) and C² continuity preventing numerical artifacts. Symplectic Euler integration delivers second-order accuracy at 1/4 the cost of RK4, spending computation on more particles rather than higher-order integration. Delta-SPH density diffusion is non-negotiable for smooth, jitter-free motion.

**Core technologies:**
- **Wendland C2 Kernel** (density/pressure/viscosity) — Compact support enables efficient neighbor search, C² continuity prevents instabilities, proven superior to cubic spline for visual applications
- **Symplectic Euler Integration** (predictor-corrector) — Time-reversible and energy-conserving, 4× cheaper than RK4, well-matched to WCSPH characteristics
- **Octree Radius Query** (neighbor search) — Extend existing Octree2D with queryRadius() method, adapts to non-uniform distributions, O(n log n) complexity
- **Tait Equation of State** (pressure solver) — Standard for WCSPH, speed of sound c = 20√(gH) limits density variation to ~1%, gamma = 7 for water-like behavior
- **Delta-SPH Density Diffusion** (stability) — Essential to eliminate pressure noise, delta = 0.1-0.2, transforms jittery simulation into smooth fluid motion
- **Dynamic Boundary Particles** (solid walls) — Fixed particles contribute to density/pressure, prevent penetration, robust for arbitrary geometry

**Critical parameters:**
- Smoothing length h = 1.2-1.5 × particle_spacing (target 30-50 neighbors in 2D)
- Artificial speed of sound c = 20√(gH) where H is characteristic height
- CFL-based adaptive timestep dt = 0.4 × min(h/v_max, h/c_sound)
- Artificial viscosity alpha = 0.01-0.05 (water-like to honey-like)

### Expected Features

The research identifies a clear distinction between "table stakes" features that make it look like fluid versus "differentiators" that make it look impressive. The core WCSPH pressure solver with proper kernels is the foundation—without this, particles either overlap (gas-like) or remain rigid (solid-like). Viscosity forces are critical for coherent flow rather than bouncing balls. Boundary handling prevents leaks through walls. These seven table stakes are necessary to produce recognizable fluid behavior.

**Must have (table stakes):**
- WCSPH pressure solver (Tait EOS) — enforces near-incompressibility, creates liquid pushback behavior
- SPH kernel functions (Poly6/Spiky/Viscosity) — weight particle contributions by distance, prevent clumping
- Efficient neighbor search (Octree radius query) — enables 5,000+ particles at 60fps vs 100 with brute force
- Boundary handling (solid walls) — prevents fluid penetration, contains dam break scenario
- Viscosity forces — creates internal friction, makes fluid flow smoothly rather than splatter
- Gravity — drives dam break collapse and flow dynamics
- Time integration (adaptive timestep) — updates positions/velocities, prevents explosions via CFL condition

**Should have (competitive):**
- Metaball rendering (marching squares) — transforms individual particles into unified fluid surface, "mercury-like" appearance
- Interactive parameter tuning — runtime sliders for viscosity/stiffness/gravity enable live experimentation
- Color field visualization — map pressure/velocity to color gradient, looks scientific and impressive
- Interactive obstacle placement — click to place solid objects, watch fluid flow around them
- Splashing/spray particles — detect high-velocity surface particles, render as mist for violent impacts
- Surface tension — smooth droplets, cohesive surface, beading effects (computationally expensive, may skip)
- Multiple fluid phases — water/oil separation, visually striking but complex (defer to v2+)

**Defer (v2+):**
- GPU acceleration (thousands to millions of particles)
- 3D extension (architecture supports but needs separate implementation)
- Advanced rendering (screen-space refraction, caustics)
- Multi-phase interactions with proper interface tracking

**Anti-features (avoid):**
- Excessive artificial viscosity (α > 0.1) — overdamps motion, fluid looks syrupy
- Fixed large timesteps — ignoring CFL condition causes explosions
- All-pairs neighbor search O(n²) — unusable beyond 100 particles
- Gaussian kernels — infinite support defeats spatial partitioning
- RK4 for fluids — 4× cost for minimal visual improvement, better to use more particles

### Architecture Approach

SPH integrates cleanly into the existing 5-layer engine architecture without requiring major restructuring. The key design decision is adding SPHSolver as a standalone component in the Physics layer (similar to Force), keeping particles as pure data carriers rather than embedding SPH logic in Particle2D methods. System owns the SPHSolver instance and orchestrates the 6-step pipeline in its Advect() method: spatial structure rebuild → density/pressure computation → SPH forces → external forces → boundaries → integration. This ordering respects critical data dependencies (must compute density before pressure, pressure before forces).

**Major components:**
1. **SPHSolver** (new, Physics layer) — Encapsulates WCSPH pipeline: ComputeDensityPressure() and ComputeForces() methods, stores simulation parameters (rest_density, stiffness, viscosity, smoothing_length), reusable across scenes
2. **Particle2D extensions** (existing, add SPH properties) — Add density, pressure, pressureForce, viscosityForce fields, existing methods delegate to SPHSolver
3. **Octree2D radius query** (extend existing) — Add queryRadius(center, radius) method to find neighbors within smoothing length h, cell size ≈ h for optimal performance
4. **SPHKernel functions** (already implemented) — Poly6 for density, Spiky gradient for pressure, Viscosity Laplacian for viscosity, already have formulas ready
5. **System::Advect() integration** — Insert SPH pipeline when FLUID_DYNAMICS_ENABLED, coexists with existing forces (gravity, friction, drag)
6. **Constants consolidation** (Constants.hpp) — Add SPH_SMOOTHING_LENGTH, SPH_PARTICLE_MASS, REST_DENSITY=1000, STIFFNESS, VISCOSITY parameters

**Update pipeline order (critical dependencies):**
1. Spatial structure update → Octree rebuild for current positions
2. Density & pressure → Requires neighbors, feeds into forces
3. SPH forces → Requires density/pressure from step 2
4. External forces → Gravity, friction, drag (existing)
5. Boundary conditions → Screen collisions (existing)
6. Integration → RK4 or symplectic Euler consumes accumulated forces

**3D-ready abstractions:**
- Template-based solver (Phase 2): SPHSolver<ParticleType, SpatialStructure>
- Kernel functions already dimension-agnostic (just need Vec3 overloads)
- Particle3D needs SPH properties added (density, pressure fields)
- Octree3D implementation (8 children vs 4, same interface pattern)

### Critical Pitfalls

Research identifies stability, performance, and visual quality as the three critical failure modes. Pressure oscillations are the #1 stability killer—WCSPH inherently produces noisy pressure fields that cause jittery, unrealistic motion. Delta-SPH density diffusion is not optional; it's the difference between a working simulation and garbage. Neighbor search efficiency determines whether you can hit 60fps targets; 80-90% of CPU time is spent finding neighbors, making this the bottleneck. Visual quality failures come from parameter mistuning: excessive viscosity makes fluid look thick and syrupy, wrong smoothing length causes either instability (too small) or over-damping (too large).

1. **Pressure Oscillations (Stability)** — WCSPH produces false pressure noise causing jitter and explosions. **Prevention:** Always implement delta-SPH density diffusion (delta = 0.1-0.2), use Shepard filter for kernel correction, consider Riemann solvers for high-Reynolds flows. **Phase:** Core density/pressure calculation, Phase 1.
2. **Inefficient Neighbor Search (Performance)** — Naive O(n²) kills performance above 100 particles, even spatial subdivision can be poorly implemented. **Prevention:** Use compact spatial hashing or octree radius query (O(n log n)), cell size = h, pre-sort particles for cache coherence, avoid hash collisions. Neighbor search consumes 80-90% of simulation time. **Phase:** Core data structure, must be correct before testing with many particles.
3. **CFL Condition Violations (Stability)** — Too large timesteps cause instability and explosions, too small wastes computation. **Prevention:** Calculate dt = 0.4 × min(h/v_max, sqrt(h/a_max), h/c_sound) every frame, use CFL factor 0.2-0.4 not 1.0, monitor max velocity. **Phase:** Time integration, implement early.
4. **Excessive Artificial Viscosity (Visual Quality)** — Too much viscosity (α > 0.1) makes fluid look thick/syrupy, suppresses splashes and turbulence. **Prevention:** Start with α = 0.01-0.05, only increase if penetration occurs, prefer density diffusion over artificial viscosity for stability. **Phase:** Viscosity implementation and parameter tuning.
5. **Equation of State Stiffness (Stability/Performance)** — Wrong speed of sound causes either visible compression (too low) or tiny timesteps (too high). **Prevention:** Set c = 20√(gH) minimum where H is fluid height, target density variation <1%, never use real water speed of sound (1480 m/s forces impractical timesteps). **Phase:** Pressure solver implementation.

**Secondary pitfalls to watch:**
- Tensile instability (particle clumping) — mitigate with particle shifting or background pressure
- Kernel truncation at surfaces — use Shepard filter or CSPM corrections
- Boundary pressure oscillations — smooth boundary forces, use modified dynamic boundary conditions
- Cache-unfriendly memory access — use Structure-of-Arrays (SoA) layout, sort particles spatially
- Poor initial particle distribution — use hexagonal/staggered grid in 2D, run relaxation phase before simulation start

## Implications for Roadmap

Based on research, the implementation follows natural technical dependencies: you cannot compute forces without pressure, cannot compute pressure without density, cannot compute density without neighbor search, and cannot optimize neighbor search without understanding SPH access patterns. The roadmap prioritizes getting stable fluid motion first (table stakes features), then adding visual polish (differentiators), then optimizing performance. Each phase delivers a visible milestone and addresses specific pitfalls identified in research.

### Phase 1: Core SPH Mechanics (Foundation)
**Rationale:** Establish foundational SPH machinery before attempting forces or integration. Kernel functions and neighbor search are prerequisites for all SPH calculations. Density/pressure must work correctly before forces can be meaningful.

**Delivers:** Working density and pressure calculations, validated against static uniform particle grids showing rest_density ≈ 1000 kg/m³.

**Addresses Features:**
- SPH kernel functions (Wendland C2: W and ∇W)
- Efficient neighbor search (Octree2D::queryRadius() extension)
- Density calculation (summation loop with Poly6 kernel)
- Pressure calculation (Tait equation of state)

**Avoids Pitfalls:**
- Implements neighbor search correctly from start (Pitfall #2)
- Sets equation of state parameters properly (Pitfall #5)
- Validates with static test before dynamic simulation

**Research Flag:** Standard patterns, no additional research needed. Well-documented in STACK.md with exact formulas.

---

### Phase 2: Forces and Integration (Motion)
**Rationale:** Now that density/pressure exist, forces can be computed. Integration must respect CFL conditions from the start to prevent stability issues. This phase produces visible fluid motion for the first time.

**Delivers:** Falling, splatting fluid blob that deforms realistically under gravity and pressure.

**Addresses Features:**
- Pressure forces (symmetric force formula with Spiky gradient)
- Viscosity forces (velocity diffusion with Viscosity Laplacian)
- Symplectic Euler integration (predictor-corrector)
- Adaptive timestep (CFL condition: dt = 0.4 × min(h/v_max, h/c_sound))

**Avoids Pitfalls:**
- CFL-based adaptive timestep prevents explosions (Pitfall #3)
- Low artificial viscosity α = 0.02 prevents over-damping (Pitfall #4)

**Research Flag:** Standard patterns, implementation follows STACK.md algorithm. May need parameter tuning experimentation.

---

### Phase 3: Boundaries and Containment (Dam Break Scenario)
**Rationale:** Fluid must interact with solid walls to demonstrate dam break. Boundary handling is complex and warrants separate phase after core SPH works.

**Delivers:** Fluid contained in rectangular tank, dam break simulation runs without leaks.

**Addresses Features:**
- Boundary handling (dynamic boundary particles for walls)
- Dam break geometry setup
- Screen collision integration with SPH

**Avoids Pitfalls:**
- Boundary pressure oscillations (smooth boundary forces)
- Ghost particle placement issues (use boundary particles not ghosts for rectangular geometry)

**Research Flag:** Boundaries are "most difficult technical point" of SPH according to literature. May need `/gsd:research-phase` if issues arise with complex geometry. For rectangular tank: standard patterns sufficient.

---

### Phase 4: Stability Enhancements (Critical Polish)
**Rationale:** Basic SPH will exhibit pressure noise and jitter. Delta-SPH density diffusion transforms unusable simulation into production-quality. This is non-negotiable for visual applications.

**Delivers:** Smooth, jitter-free fluid motion that looks professionally rendered. Dam break runs stable for 10+ seconds.

**Addresses Features:**
- Delta-SPH density diffusion (delta = 0.1, essential for stability)
- Background pressure (prevents negative pressure / tensile instability)
- Parameter tuning for visual quality

**Avoids Pitfalls:**
- Pressure oscillations eliminated (Pitfall #1, the #1 killer)
- Tensile instability / particle clumping prevented

**Research Flag:** Delta-SPH implementation is well-documented in STACK.md with exact formulas. Parameter tuning (delta, background pressure) may need experimentation but ranges are known.

---

### Phase 5: Performance Optimization (Hitting 60fps Target)
**Rationale:** After fluid works and looks good, profile and optimize to hit 5,000 particles at 60fps. Neighbor search is known bottleneck (80-90% of time).

**Delivers:** Validated performance meeting 5,000 particles @ 60fps on modern CPU.

**Addresses Features:**
- Neighbor search profiling and optimization
- Memory layout optimization (Structure-of-Arrays if needed)
- Pre-computation of kernel normalizations
- Symmetric force calculation (avoid duplicate pair work)

**Avoids Pitfalls:**
- Inefficient neighbor search (Pitfall #2, the performance killer)
- Cache-unfriendly memory access (SoA layout, spatial sorting)
- Redundant neighbor pair calculations (symmetric forces)

**Research Flag:** Performance tuning requires profiling actual implementation. May need architecture-specific optimization research (SIMD, cache tuning) based on profile results.

---

### Phase 6: Visual Differentiators (Impressive Demo)
**Rationale:** Core fluid simulation works and performs well. Now add visual polish that transforms particles into unified fluid surface.

**Delivers:** Production-quality visual presentation suitable for showcase video.

**Addresses Features:**
- Metaball rendering (marching squares for smooth surface)
- Interactive parameter tuning (runtime sliders for viscosity, stiffness)
- Color by pressure/velocity (visual debugging and scientific look)
- Particle rendering optimization (batch SDL2 calls)

**Avoids Pitfalls:**
- Metaball rendering becoming bottleneck (Pitfall #12, may need to use simpler point-based rendering if marching squares too slow)

**Research Flag:** Marching squares implementation may need additional research for performance. Screen-space rendering techniques may need investigation if metaballs too slow.

---

### Phase 7: Interactive Features (Optional Polish)
**Rationale:** After core showcase works, add interactive elements that make demo engaging.

**Delivers:** Playable demo where users can experiment with fluid behavior.

**Addresses Features:**
- Interactive obstacle placement (click to place solid objects)
- Particle spawning/removal (mouse interaction)
- Parameter presets (quick switching between water/honey/etc)

**Research Flag:** Standard input handling patterns, no additional research needed.

---

### Phase Ordering Rationale

The phase structure follows strict technical dependencies from research:
- **Phases 1-2 are sequential:** Cannot compute forces without density/pressure, cannot integrate without forces
- **Phase 3 builds on 1-2:** Boundaries require working pressure forces to prevent penetration
- **Phase 4 is critical after 3:** Delta-SPH must be added before extensive testing, transforms simulation quality
- **Phase 5 after 4:** Cannot optimize until behavior is correct, profiling requires stable baseline
- **Phases 6-7 are polish:** Visual and interactive features are independent, can be reordered or deferred

The grouping reflects architecture patterns from ARCHITECTURE.md:
- Phases 1-2: SPHSolver core implementation
- Phase 3: Boundary handling extension
- Phase 4: Numerical stability (delta-SPH is algorithm-level enhancement)
- Phase 5: Performance optimization (data structure and memory layout)
- Phases 6-7: Rendering and interaction layers (separate from solver)

This ordering avoids the critical pitfalls by addressing them in priority order:
1. Phase 2 prevents Pitfall #3 (CFL violations) immediately
2. Phase 4 fixes Pitfall #1 (pressure oscillations) before extensive feature work
3. Phase 5 addresses Pitfall #2 (neighbor search) with profiling-driven optimization
4. Viscosity tuning in Phase 2 prevents Pitfall #4 (overdamping)
5. EOS parameters in Phase 1 prevent Pitfall #5 (stiffness issues)

### Research Flags

**Phases likely needing deeper research during planning:**
- **Phase 3 (Boundaries):** If moving beyond rectangular tank to arbitrary geometry, boundary particle placement becomes complex. Research shows this is "most difficult technical point of SPH." For simple rectangular dam break: standard patterns sufficient.
- **Phase 5 (Performance):** If profiling reveals unexpected bottlenecks beyond neighbor search. May need architecture-specific optimization research (cache tuning, SIMD vectorization) based on actual profile data.
- **Phase 6 (Metaballs):** If marching squares performance inadequate. May need to research screen-space rendering, point-based splatting, or GPU acceleration approaches.

**Phases with standard patterns (skip research-phase):**
- **Phase 1 (Core SPH):** Kernel functions and density/pressure calculations are well-documented with exact formulas in STACK.md
- **Phase 2 (Forces/Integration):** Symplectic Euler and SPH force equations are standard, implementation is straightforward
- **Phase 4 (Stability):** Delta-SPH formulation is clearly documented with parameter ranges
- **Phase 7 (Interactive):** Standard input handling and UI patterns

## Confidence Assessment

| Area | Confidence | Notes |
|------|------------|-------|
| Stack | **HIGH** | Convergent recommendations from multiple authoritative sources (DualSPHysics, SPlisHSPlasH, academic papers). Wendland C2 + symplectic Euler + delta-SPH is proven production stack. Parameter values well-established. |
| Features | **HIGH** | Clear distinction between table stakes and differentiators based on multiple SPH library documentation. WCSPH requirements are well-understood. Metaball rendering complexity documented in GPU Gems and research papers. |
| Architecture | **HIGH** | Integration points clear from existing codebase inspection. SPHSolver pattern follows established Physics layer conventions. 6-step pipeline is standard for WCSPH implementations. 3D extension path validated against similar projects. |
| Pitfalls | **HIGH** | Pitfalls derived from production implementation experience (DualSPHysics forums, SPlisHSPlasH issues, academic papers on stability). Pressure oscillations and neighbor search are documented recurring issues with known solutions. |

**Overall confidence:** **HIGH**

The research synthesizes information from production SPH codes (DualSPHysics, SPlisHSPlasH), authoritative academic papers (Dehnen & Aly 2012 on kernels, Antuono et al. 2010 on delta-SPH), comprehensive implementation guides (Interactive Computer Graphics SPH tutorial, GPUSPH Theory Guide), and physics simulation textbooks. All sources converge on the same recommendations for WCSPH configuration and pitfalls.

The existing codebase provides strong constraints and opportunities: Particle2D already has SPH property stubs, Octree2D exists for neighbor search, SPHKernel.cpp has formula implementations, System class has FLUID_DYNAMICS_ENABLED toggle. These reduce implementation risk compared to greenfield development.

### Gaps to Address

**Parameter sensitivity and interdependencies:** Research provides recommended ranges (e.g., alpha = 0.01-0.05, delta = 0.1-0.2) but optimal values depend on specific particle count, domain size, and visual targets. **Mitigation:** Phase 4 includes systematic parameter tuning with validation against dam break scenario. Consider implementing parameter presets for common scenarios.

**Rendering performance trade-offs:** Metaball rendering via marching squares may become bottleneck at target particle counts (5,000). Research identifies this as potential issue but doesn't provide clear performance model for SDL2 CPU rendering. **Mitigation:** Prototype multiple rendering approaches in Phase 6 (marching squares, point splatting, simple circles with alpha blending). Profile early with target particle counts. Be prepared to use simpler rendering if marching squares too expensive.

**Boundary handling complexity:** Research emphasizes boundaries are "most difficult technical point" but rectangular tank geometry (dam break scenario) is simpler than arbitrary geometry. Unclear if ghost particles, boundary particles, or penalty forces will be most robust. **Mitigation:** Phase 3 starts with simplest approach (boundary particles for rectangular walls). Flag for deeper research if issues arise with corners or penetration.

**3D extension timeline:** Architecture is designed to support 3D but Particle3D lacks SPH properties, Octree3D doesn't exist, and kernel normalizations need 3D constants. Scope of 3D work is estimable but not detailed. **Mitigation:** Defer 3D to separate milestone after 2D validated. ARCHITECTURE.md documents template-based approach for future refactor.

**Initial particle distribution impact:** Research shows hexagonal/staggered grids outperform square grids but doesn't quantify performance difference for dam break scenario. May need relaxation phase before simulation start. **Mitigation:** Start with simple square grid, monitor initial density uniformity. Switch to hexagonal if initial instability observed. Phase 1 includes static validation test.

## Sources

### Primary (HIGH confidence)
- **STACK.md** — Derived from DualSPHysics documentation, Dehnen & Aly 2012 (kernels), Interactive Computer Graphics SPH tutorial, GPUSPH Theory Guide
- **Dehnen & Aly (2012)** — Optimized smoothing kernels for SPH (Wendland C2 recommendation)
- **Antuono et al. (2010)** — Delta-SPH density diffusion formulation
- **DualSPHysics Forums** — Production WCSPH implementation details, time-stepping formulas, CFL conditions
- **SPlisHSPlasH** — Open-source reference implementation, features documentation
- **Interactive Computer Graphics** — WCSPH formulation, parameter guidelines

### Secondary (MEDIUM confidence)
- **FEATURES.md** — Derived from Müller et al. 2003 (foundational SPH paper), SPlisHSPlasH features, GPU Gems metaball rendering
- **Longva et al. (2022)** — Octree neighbor search for SPH, recent research showing competitive performance
- **ARCHITECTURE.md** — Validated against existing PhysicsEngine codebase structure
- **Kelager (2006)** — Beginner-friendly SPH tutorial (dated but covers fundamentals)

### Tertiary (LOW confidence)
- **PITFALLS.md** — Aggregated from forums, Medium articles, and older papers; some anecdotal evidence
- **Marching squares performance** — GPU Gems provides GPU implementation, unclear how SDL2 CPU performance scales
- **Cache optimization specifics** — General principles documented, architecture-specific tuning may differ

---
*Research completed: 2026-01-23*
*Ready for roadmap: yes*
