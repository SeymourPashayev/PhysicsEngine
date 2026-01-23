# SPH Pitfalls

Common mistakes and how to avoid them when implementing WCSPH fluid simulation for real-time visual applications.

## Numerical Stability

### 1. Pressure Oscillations and Particle Explosions

**Problem**: The standard WCSPH formula produces false pressure oscillations when the flow is inviscid or has high Reynolds numbers, leading to unstable numerical calculations and particles "exploding" apart.

**Root Cause**: High random pressure oscillations in the pressure field due to noisy density calculations and lack of numerical damping.

**Warning Signs**:
- Particles suddenly accelerating to extreme velocities
- Visible "jittering" or vibration in particle positions
- Simulation becoming unstable within first few seconds
- Pressure values fluctuating wildly between timesteps

**Prevention Strategy**:
- **Density Diffusion** (Critical): Always implement density diffusion terms to remove numerical noise in the pressure field. This is highly recommended for all WCSPH implementations.
- **Delta-SPH or Riemann Solvers**: Consider using Riemann-based SPH formulations or delta-SPH schemes that are designed to handle pressure oscillations.
- Use smoothing or filtering on pressure values (e.g., Shepard filter).

**Phase to Address**: Core SPH algorithm implementation phase (density/pressure calculation)

**References**:
- [Weakly Compressible SPH - TrixiParticles.jl](https://trixi-framework.github.io/TrixiParticles.jl/stable/systems/weakly_compressible_sph/)
- [Study of WCSPH for violent fluid-structure impact](https://iwaponline.com/ws/article/22/6/5771/88786/Study-of-the-weakly-compressible-SPH-method-for)

### 2. Tensile Instability and Particle Clumping

**Problem**: Particles form unphysical clusters or clumps, especially under negative pressure conditions, leaving void regions in the fluid.

**Root Cause**: The standard SPH method suffers from tensile instability under tension (negative pressure), causing particles to attract each other unnaturally.

**Warning Signs**:
- Particles forming dense clusters separated by empty gaps
- "Stringy" or "blobby" fluid appearance instead of smooth flow
- Voids appearing in regions that should contain fluid
- Particle distribution becoming increasingly non-uniform over time

**Prevention Strategy**:
- Implement **particle shifting** techniques to maintain uniform particle distribution
- Use **Transport Velocity Formulation** in the momentum equation to eliminate non-physical voids
- Apply artificial pressure or tensile correction terms
- Consider XSPH velocity smoothing to reduce particle disorder

**Phase to Address**: Core SPH algorithm (pressure/force calculation) + particle distribution correction phase

**References**:
- [WCSPH Particle Shifting Strategy](https://www.mdpi.com/2073-4441/12/11/3189)
- [Stability and accuracy of WCSPH with particle regularization](https://www.sciencedirect.com/science/article/abs/pii/S0997754622000450)

### 3. CFL Condition Violations and Timestep Issues

**Problem**: Using too large a timestep causes numerical instability, while overly conservative timesteps waste computation.

**Root Cause**: The CFL (Courant-Friedrichs-Lewy) condition requires that information propagates at most one cell per timestep. Violating this causes instability.

**Warning Signs**:
- Particles moving more than one smoothing length (h) per timestep
- Stability improving when timestep is reduced by 50%
- Particles "tunneling" through boundaries
- Sudden energy increases in the system

**Prevention Strategy**:
- Calculate timestep based on: `dt = CFL_factor * min(h / v_max, sqrt(h / a_max), h / c_s)`
  where v_max is maximum velocity, a_max is maximum acceleration, c_s is speed of sound
- Use CFL_factor = 0.2 to 0.4 (NOT 1.0) for safety margin
- Implement adaptive timestepping that adjusts dt dynamically
- For real-time applications at 60fps, verify that required dt doesn't exceed 1/60s

**Phase to Address**: Time integration phase (should be implemented early)

**References**:
- [CFL Condition - SimScale](https://www.simscale.com/blog/cfl-condition/)
- [Time Adaptive Approximate SPH](https://maverick.inria.fr/~Prashant.Goswami/Research/Papers/VRIPHYS11_TAASPH.pdf)

### 4. Equation of State Stiffness Parameter

**Problem**: Incorrect equation of state parameters cause either excessive compressibility (unrealistic fluid) or numerical instability (too stiff).

**Root Cause**: The stiffness parameter in Tait's equation of state controls the pressure-density relationship. Too high causes tiny timesteps; too low causes visible compression.

**Warning Signs**:
- Fluid visibly compressing (density variations > 1-2%)
- Simulation requiring extremely small timesteps to remain stable
- Particles overlapping significantly
- Speed of sound being less than 10x maximum fluid velocity

**Prevention Strategy**:
- Set artificial speed of sound c_s to **at least 10x** the maximum expected fluid velocity
- For dam break: use c_s = 20 * sqrt(g * H) where H is initial fluid height
- Target density variation < 1% (WCSPH typically allows 1-3%)
- Use gamma = 7 for water-like fluids in Tait's equation
- DO NOT use the real speed of sound in water (~1480 m/s) - this forces impractically small timesteps

**Phase to Address**: Equation of state implementation phase

**References**:
- [WCSPH - Interactive Computer Graphics](https://interactivecomputergraphics.github.io/physics-simulation/examples/wcsph.html)
- [DualSPHysics SPH Formulation](https://github.com/DualSPHysics/DualSPHysics/wiki/3.-SPH-formulation)

### 5. Kernel Truncation at Free Surfaces

**Problem**: Particles near the free surface have incomplete kernel support (missing neighbors), leading to incorrect density/pressure calculations and particle deficiency.

**Root Cause**: The SPH kernel integration assumes a full spherical neighborhood, but surface particles only have neighbors on one side.

**Warning Signs**:
- Surface particles having lower calculated density than interior particles
- Particles "evaporating" from the free surface
- Wavy or bumpy surface appearance when it should be smooth
- Pressure errors concentrated at the fluid surface

**Prevention Strategy**:
- Implement **Shepard filter** (kernel correction) to normalize incomplete kernel support
- Use corrective SPH (CSPM) formulations for better consistency
- Consider free surface detection and special treatment for surface particles
- Ensure smoothing length h provides sufficient neighbors (target 20-40 neighbors per particle)

**Phase to Address**: Density calculation phase + surface treatment phase

**References**:
- [SPH Consistency - Frontiers](https://www.frontiersin.org/journals/applied-mathematics-and-statistics/articles/10.3389/fams.2021.797455/full)
- [New kernel function for free surface flows](https://www.sciencedirect.com/science/article/pii/S0307904X13007920)

## Performance

### 6. Inefficient Neighbor Search (90% of Runtime)

**Problem**: Naive O(N²) neighbor search makes simulation extremely slow. Even with spatial subdivision, poor implementation kills performance.

**Root Cause**: The neighbor search is the performance bottleneck, consuming **80-90% of total simulation time** in most SPH implementations.

**Warning Signs**:
- Simulation slowing down dramatically as particle count increases
- Profiling shows most time spent in neighbor finding, not force calculation
- Hash collisions causing linear searches through long lists
- Poor cache locality causing memory stalls

**Prevention Strategy**:
- **Use compact spatial hashing** (NOT naive uniform grid or simple hashing)
- Cell size should equal smoothing length h (one kernel radius)
- Pre-sort particles by hash value for better cache coherence
- On CPU: Use Z-order curve (Morton code) for spatial sorting
- Consider octree for highly non-uniform distributions
- Compressed neighbor lists can outperform compact hashing
- For your target (1K-5K particles): uniform grid or compact hash is sufficient

**Phase to Address**: Core data structure phase (must be implemented early, before testing with many particles)

**References**:
- [Parallel SPH on multi-core CPUs](https://cg.informatik.uni-freiburg.de/publications/2011_CGF_dataStructuresSPH.pdf)
- [Optimization of neighbor lists on GPU](https://www.sciencedirect.com/science/article/pii/S0965997824001182)
- [SPH and Hashing - Medium](https://medium.com/@ahmed.fathy.elbossily/sph-and-hashing-c28e70ff7615)

### 7. Cache-Unfriendly Memory Access Patterns

**Problem**: Random memory access patterns cause cache misses, slowing simulation by 2-5x even with good neighbor search.

**Root Cause**: SPH accesses particle data in neighbor-driven order, not sequential order, causing poor cache utilization.

**Warning Signs**:
- Profiler shows high cache miss rates (>10%)
- Performance not scaling with CPU clock speed
- Memory bandwidth saturation
- Better performance on older CPUs with larger caches

**Prevention Strategy**:
- Use **Structure of Arrays (SoA)** layout, NOT Array of Structures (AoS)
- Sort particles spatially every frame (using Z-curve/Morton codes)
- Store frequently accessed data (position, velocity) separately from rarely accessed data (color, ID)
- Pre-allocate fixed-size neighbor lists instead of dynamic allocation
- Process particles in spatial order matching neighbor search

**Phase to Address**: Data structure design phase (early) + optimization phase (after basic implementation works)

**References**:
- [Parallel SPH implementation on CPUs](https://cg.informatik.uni-freiburg.de/publications/2011_CGF_dataStructuresSPH.pdf)

### 8. Redundant Neighbor Pair Calculations

**Problem**: Calculating force between particle A and B twice (once from A's perspective, once from B's perspective) wastes 50% of computation.

**Root Cause**: Naive implementations iterate through each particle's neighbors independently, causing duplicate work by Newton's 3rd law.

**Warning Signs**:
- Force calculation time scales as O(N²) even with spatial subdivision
- Each particle pair contributing to forces twice
- Memory bandwidth dominated by reading neighbor data

**Prevention Strategy**:
- Implement **symmetric force calculation**: when processing pair (A,B), compute force on both A and B simultaneously
- Store forces in separate arrays and accumulate atomically (if parallelizing)
- For single-threaded: use "forward" neighbor iteration (only process B if B.id > A.id)
- This cuts force calculation cost in half

**Phase to Address**: Force calculation phase

### 9. Smoothing Length Selection

**Problem**: Wrong smoothing length h causes either too few neighbors (noisy/unstable) or too many neighbors (slow/overdamped).

**Root Cause**: Smoothing length controls kernel radius and therefore neighbor count. Sweet spot is narrow.

**Warning Signs**:
- Particles having < 15 neighbors (too small h): noisy, unstable simulation
- Particles having > 50 neighbors (too large h): slow performance, excessive damping
- Simulation results changing drastically with small h adjustments

**Prevention Strategy**:
- Set h = 1.3 * particle_spacing initially (common ratio)
- Target **20-40 neighbors per particle** for 2D
- For uniform distributions: h ≈ 2.0 * particle_spacing works well
- Variable smoothing length adds complexity - only use if truly needed
- Monitor average neighbor count and adjust h if outside target range

**Phase to Address**: Initial parameter tuning phase

**References**:
- [WCSPH - TrixiParticles.jl](https://trixi-framework.github.io/TrixiParticles.jl/stable/systems/weakly_compressible_sph/)
- [Variable smoothing length method](https://link.springer.com/article/10.1186/s42492-022-00128-x)

## Visual Quality

### 10. Excessive Artificial Viscosity Overdamping

**Problem**: Using too much artificial viscosity makes fluid look "thick" or "syrupy", suppresses splashes and turbulence, erases surface detail.

**Root Cause**: Artificial viscosity is needed for stability but overdamps physically correct behavior if set too high.

**Warning Signs**:
- Fluid appears too viscous (honey-like instead of water-like)
- Splashes and droplets not separating from main body
- No turbulence or swirling motion visible
- Flow patterns looking unnaturally smooth
- Rotating flows losing angular momentum quickly

**Prevention Strategy**:
- Start with **low** artificial viscosity parameter (α = 0.01 to 0.05 for visual applications)
- Only increase if you observe penetration or instability
- Modern SPH may not need artificial viscosity at all if using:
  - Riemann solvers
  - EDAC (Entropically Damped Artificial Compressibility) methods
  - Proper density diffusion
- For computer graphics: prefer physical viscosity over artificial viscosity when possible
- Use time-dependent artificial viscosity (activates only when needed)

**Phase to Address**: Viscosity implementation + parameter tuning phase

**References**:
- [Are heuristic switches necessary for artificial viscosity?](https://arxiv.org/html/2510.20534)
- [Measuring effects of artificial viscosity](https://academic.oup.com/mnras/article/426/2/1687/976181)
- [Controlling artificial viscosity](https://www.aanda.org/articles/aa/pdf/2010/08/aa14466-10.pdf)

### 11. Surface Tension: Cohesion/Adhesion Imbalance

**Problem**: Incorrect surface tension forces cause particle clustering on the surface, "beading up" where fluid should spread, or lack of cohesion.

**Root Cause**: Existing cohesion-force methods don't always correspond to correct minimal surface area; equilibrium of forces doesn't match physical surface tension.

**Warning Signs**:
- Particles clustering in unnatural "clumps" on fluid surface
- Thin fluid sheets breaking apart too easily (insufficient cohesion)
- Droplets not forming spherical shapes
- Fluid sticking to boundaries when it shouldn't (excessive adhesion)
- Surface looking "bumpy" instead of smooth

**Prevention Strategy**:
- For visual showcase: surface tension may be **optional** - many demos skip it
- If implementing: use modern **implicit surface tension** methods (Jeske et al. 2023) rather than explicit cohesion forces
- Explicit cohesion methods are prone to clustering artifacts
- Consider that metaball rendering will smooth the surface visually, potentially masking the need for surface tension
- Start without surface tension; add only if visual quality requires it

**Phase to Address**: Advanced features phase (after basic fluid simulation works)

**References**:
- [Implicit Surface Tension for SPH](https://dl.acm.org/doi/10.1145/3631936)
- [Versatile surface tension and adhesion](https://dl.acm.org/doi/10.1145/2508363.2508395)

### 12. Metaball Rendering Performance Bottleneck

**Problem**: Marching cubes for surface extraction becomes the bottleneck, not the SPH simulation itself, at higher particle counts or grid resolutions.

**Root Cause**: Marching cubes has O(grid_volume) complexity, visiting every grid cell. Cannot exploit temporal coherence between frames.

**Warning Signs**:
- Rendering taking longer than SPH simulation
- Frame rate dropping as you increase marching cubes grid resolution
- Performance depending more on domain size than particle count
- Visual "popping" as surface vertices are recreated each frame

**Prevention Strategy**:
- For 1K-5K particles: **screen-space rendering** may be faster and simpler than marching cubes
- Point-based surface rendering has clear performance advantage (O(surface_area) not O(volume))
- If using marching cubes:
  - Limit grid resolution to what's visually necessary (64³ or 128³ max)
  - Use spatial subdivision to skip empty cells
  - Consider 2D marching squares if doing 2D simulation
- Evaluate performance early with target particle counts before committing to rendering approach

**Phase to Address**: Rendering system phase (should prototype multiple approaches early)

**References**:
- [Point-Based Metaballs on GPU - NVIDIA](https://developer.nvidia.com/gpugems/gpugems3/part-i-geometry/chapter-7-point-based-visualization-metaballs-gpu)
- [Surface Reconstruction of SPH Fluids](https://ramakarl.com/surface-reconstruction-of-sph-fluids/)
- [Real-time SPH and Marching Cubes](https://www.researchgate.net/publication/258385963_Real-Time_Simulation_of_Fluid_Scenes_by_Smoothed_Particle_Hydrodynamics_and_Marching_Cubes)

## Boundary Handling

### 13. Ghost Particle Placement in Complex Geometry

**Problem**: Ghost particles (mirror particles outside boundaries) fail at corners, sharp angles, and complex shapes, causing particles to leak through or get stuck.

**Root Cause**: Mirroring approach doesn't have clear placement rules at sharp corners or saw-tooth boundaries. Multi-layer ghost particles needed for complex geometry increase computational cost significantly.

**Warning Signs**:
- Fluid leaking through walls at corners or edges
- Particles getting "stuck" at acute angle corners
- Unphysical gaps forming between fluid and solid boundaries
- Ghost particles ending up inside the fluid domain on opposite side of sharp features

**Prevention Strategy**:
- For simple geometry (rectangular tanks): ghost particles work well
- For complex geometry: consider **dynamic boundary particles** instead of ghost particles
  - Boundary particles have their own density/pressure
  - More robust for arbitrary shapes
- Ensure sufficient ghost particle layers (typically 2-3 layers of thickness h)
- For your showcase: start with simple rectangular boundaries, add complexity only if needed
- Test corner cases explicitly (90° corners, acute angles)

**Phase to Address**: Boundary handling phase

**References**:
- [Local Semi-Fixed Ghost Particles](https://www.mdpi.com/2077-1312/9/4/416)
- [Modified Dynamic Boundary Conditions](https://link.springer.com/article/10.1007/s40571-021-00403-3)
- [Ghost particle removed (GR-SPH) method](https://www.sciencedirect.com/science/article/pii/S0307904X16304966)

### 14. Boundary Pressure Oscillations

**Problem**: Particles near boundaries experience large pressure fluctuations, causing jittering, noise, or particles being violently repelled.

**Root Cause**: In violent fluid impact, repulsive forces can be extremely large when particles approach boundaries instantly, forcing particles away with unrealistic velocities.

**Warning Signs**:
- Particles "bouncing" off boundaries with too much energy
- Visible vibration or jittering near walls
- Pressure spikes in boundary regions
- Fluid particles penetrating slightly into boundaries then being ejected

**Prevention Strategy**:
- Use **pressure smoothing** or filtering near boundaries
- Implement **modified dynamic boundary conditions (mDBC)** for better pressure distribution
- Limit maximum boundary repulsion force
- Use friction/damping at boundaries to absorb impact energy
- Ensure boundary particles have reasonable density/pressure values

**Phase to Address**: Boundary force calculation phase

**References**:
- [Dynamical pressure boundary condition](https://arxiv.org/html/2403.09485v1)
- [Modified dynamic boundary conditions](https://link.springer.com/article/10.1007/s40571-021-00403-3)

### 15. Inlet/Outlet Boundary Artifacts

**Problem**: Ghost particle techniques at inlets/outlets create void regions, disordered particles, and numerical errors.

**Root Cause**: Mirror-axes approaches for ghost particles at open boundaries violate assumptions about particle neighborhoods.

**Warning Signs**:
- Particles becoming disordered near inlets/outlets
- Void regions appearing at flow boundaries
- Density/pressure discontinuities at inlet/outlet
- Simulation instability when fluid enters/exits domain

**Prevention Strategy**:
- For visual showcase: **avoid inlets/outlets entirely** if possible
  - Use closed domains (tanks, containers)
  - Allows "infinite" fluid by just not rendering particles outside view
- If needed: use specialized inlet/outlet boundary conditions, not ghost particles
  - Buffer zones with prescribed velocity/density
  - Particle insertion/deletion strategies
- This is a complex feature - implement only if required for your demo

**Phase to Address**: Advanced boundary handling (optional feature)

**References**:
- [Modified dynamic boundary conditions - inlets/outlets](https://link.springer.com/article/10.1007/s40571-021-00403-3)

## Initialization and Debugging

### 16. Initial Particle Distribution Quality

**Problem**: Poor initial particle arrangement causes immediate instability or never achieves equilibrium, with effects persisting throughout simulation.

**Root Cause**: Regular lattice arrangements can interact poorly with SPH kernels; irregular distributions start with particle deficiency.

**Warning Signs**:
- Simulation unstable from frame zero
- Particles immediately accelerating before any external forces applied
- Initial density field showing large variations (>5%) when fluid should be at rest
- Pressure oscillations that never settle down

**Prevention Strategy**:
- Use **Body-Centered Cubic (BCC)** or **hexagonal close packing** for 3D
- Use **hexagonal/staggered grid** for 2D (better than square grid)
- Research shows **Greedy distribution** performs better than regular SC/BCC for some flows
- Initialize with h/dp ratio of ~1.3 to 2.0 (smoothing length to particle spacing)
- Run relaxation phase: let particles settle under pressure forces for several timesteps before starting actual simulation
- Verify initial density is uniform (within 1%) before proceeding

**Phase to Address**: Initialization phase (critical to get right early)

**References**:
- [Importance of Initial Particle Distribution](https://link.springer.com/article/10.1007/s12205-022-0304-1)
- [WCSPH comparison for 3D dam-break](https://arxiv.org/abs/2306.01959)

### 17. Parameter Interdependencies

**Problem**: Changing one parameter (e.g., smoothing length) requires retuning multiple other parameters (timestep, viscosity, stiffness), making the system fragile.

**Root Cause**: SPH parameters are tightly coupled. Speed of sound affects timestep, smoothing length affects neighbor count and viscosity, etc.

**Warning Signs**:
- Simulation breaks when changing a single parameter
- Need to retune everything when adjusting particle count
- No clear relationship between parameter changes and results
- Unstable simulation despite individually reasonable parameters

**Prevention Strategy**:
- Use **non-dimensional analysis** to set parameters consistently:
  - CFL number (not raw timestep)
  - h/dp ratio (smoothing length to particle spacing)
  - Mach number (v_max / c_s should be < 0.1)
- Document parameter relationships clearly
- Create parameter presets for common scenarios (dam break, pool, etc.)
- Implement parameter validation that warns about inconsistent settings
- Start with proven parameter sets from literature (e.g., DualSPHysics defaults)

**Phase to Address**: Throughout implementation + testing phase

**References**:
- [Parameter optimization by evolutionary metaheuristics](https://www.sciencedirect.com/science/article/abs/pii/S0010465519301572)
- [Parametric studies on SPH simulations](https://www.koreascience.or.kr/article/JAKO202003440878540.pdf)

---

## Quick Reference: Priority Levels

### Must Fix (Critical for basic functionality):
1. Neighbor search efficiency (#6)
2. CFL condition / timestep (#3)
3. Equation of state parameters (#4)
4. Pressure oscillations (#1)

### Should Fix (Important for quality):
5. Initial particle distribution (#16)
6. Kernel truncation at surfaces (#5)
7. Simple boundary handling (#13, #14)
8. Artificial viscosity tuning (#10)

### Nice to Have (Polish):
9. Particle shifting for tensile instability (#2)
10. Surface tension (#11)
11. Rendering optimization (#12)
12. Advanced boundaries (#15)

### Performance Optimizations (After it works):
13. Cache-friendly memory (#7)
14. Symmetric force calculation (#8)
15. Smoothing length optimization (#9)
