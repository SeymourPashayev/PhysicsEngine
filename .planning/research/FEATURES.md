# SPH Features for Dam Break Visual Showcase

## Table Stakes
*Must-have features for convincing fluid behavior*

### 1. WCSPH Pressure Solver
**Description**: Equation of State (EOS) pressure calculation that enforces near-incompressibility. Particles resist compression based on density deviation from rest density, creating the characteristic "fluid pushback" that prevents overlapping.

**Visual Impact**: Without this, particles either overlap (looks like gas) or remain completely rigid (looks like solid). This is what makes it look like liquid.

**Complexity**: Moderate
- Requires per-particle density calculation using kernel summation
- Tait equation: `P = k * ((ρ / ρ₀)^γ - 1)` where k is stiffness constant
- Stiff differential equation requires small timesteps or velocity clamping

**Dependencies**: SPH kernel functions (Poly6 for density), neighbor search

---

### 2. SPH Kernel Functions
**Description**: Smoothing functions that weight particle contributions by distance. Essential kernels:
- **Poly6** for density calculation (smooth falloff)
- **Spiky** for pressure gradient (prevents particle clumping)
- **Viscosity** kernel for diffusion forces

**Visual Impact**: Wrong kernels cause particle clumping, uneven density fields, and unstable behavior. Correct kernels produce smooth, natural fluid motion.

**Complexity**: Trivial
- Mathematical formulas from Müller et al. 2003 paper
- ~5-10 lines each for kernel value and gradient
- Pre-compute constants at initialization

**Dependencies**: None (foundation for all SPH)

**Reference**: [Particle-Based Fluid Simulation for Interactive Applications](https://matthias-research.github.io/pages/publications/sca03.pdf)

---

### 3. Efficient Neighbor Search
**Description**: Spatial data structure (grid hash or octree) to find all particles within smoothing radius (h). Brute force O(n²) kills performance above 100 particles.

**Visual Impact**: No direct visual impact, but enables real-time simulation. Without it, frame rate drops to unusable levels.

**Complexity**: Moderate (Octree2D already exists, needs adaptation for SPH)
- Uniform grid hashing is simpler: partition space into h×h cells
- Each particle queries 9 cells in 2D (3×3 neighborhood)
- Rebuild structure each frame or track particle movement

**Dependencies**: None (but Octree2D exists in codebase)

**Performance**: Enables 5,000+ particles at 60fps vs ~100 particles with brute force

---

### 4. Boundary Handling (Solid Walls)
**Description**: Prevents fluid from penetrating container walls. Methods:
- **Boundary particles**: Fixed particles along walls that contribute to pressure
- **Penalty forces**: Push particles back when they approach walls
- **Ghost particles**: Mirror particles on the other side of boundary

**Visual Impact**: Without this, fluid leaks through walls or behaves weirdly at boundaries. Proper boundaries make the dam break look contained and realistic.

**Complexity**: Moderate
- Boundary particles are simplest: create fixed particles at wall locations
- Need to handle corners properly to avoid gaps
- Requires tuning of boundary particle spacing

**Dependencies**: Pressure solver, kernel functions

**Note**: "Treatment of boundary conditions is certainly one of the most difficult technical points of the SPH method" but necessary for dam break scenario.

---

### 5. Viscosity Forces
**Description**: Velocity diffusion between neighboring particles creates internal friction, making fluid flow smoothly rather than splattering like individual projectiles.

**Visual Impact**: Without viscosity, water behaves like bouncy balls. With it, fluid flows coherently with smooth velocity fields. Critical for "liquid-like" appearance.

**Complexity**: Trivial
- Artificial viscosity: force proportional to velocity difference
- `F_visc = μ * Σ(v_j - v_i) * W_viscosity(r_ij)`
- Requires viscosity kernel and one loop over neighbors

**Dependencies**: Viscosity kernel, neighbor search

---

### 6. Gravity
**Description**: Constant downward acceleration on all particles.

**Visual Impact**: Makes fluid fall, drives the dam break collapse. Without it, fluid just sits there.

**Complexity**: Trivial (already exists in codebase)

**Dependencies**: None

---

### 7. Time Integration
**Description**: Updates particle positions and velocities each frame. Methods: Euler, Leapfrog, or Runge-Kutta.

**Visual Impact**: Poor integration causes energy gain (particles explode) or numerical damping (fluid loses energy and settles too quickly).

**Complexity**: Trivial (RK4 already exists in codebase)
- Symplectic Euler or Leapfrog preferred for SPH
- Adaptive timestep based on CFL condition: `Δt ≤ 0.25 * h / v_max`

**Dependencies**: All force calculations

---

## Differentiators
*Nice-to-have features that make it look impressive*

### 8. Metaball Rendering (Marching Squares)
**Description**: Instead of rendering particles as circles, reconstruct smooth fluid surface using implicit surface (metaballs) and polygonize with marching squares. Creates blob-like, mercury-like fluid appearance.

**Visual Impact**: Transforms individual particles into unified fluid surface. Highly impressive visual upgrade. Goes from "particles" to "actual liquid."

**Complexity**: Complex
- Evaluate metaball field on 2D grid: `f(x,y) = Σ(r² / |p_i - (x,y)|²)`
- Apply marching squares to extract iso-contour at threshold value
- Render polygonal mesh instead of circles
- Performance intensive: needs optimized evaluation (only near particles)

**Dependencies**: None (pure rendering concern)

**Alternative**: Point splatting (render particles as textured quads) is simpler but less impressive

**References**:
- [Point-Based Visualization of Metaballs on GPU](https://developer.nvidia.com/gpugems/gpugems3/part-i-geometry/chapter-7-point-based-visualization-metaballs-gpu)
- [Surface Reconstruction of SPH Fluids](https://ramakarl.com/surface-reconstruction-of-sph-fluids/)

---

### 9. Surface Tension
**Description**: Cohesive forces at fluid surface that minimize surface area. Creates beading effects, smooth surface, and prevents artificial separation.

**Visual Impact**: Fluid forms smooth droplets rather than irregular clumps. Surface looks "wet" and cohesive. Nice for small-scale details.

**Complexity**: Moderate to Complex
- Color-field method: detect surface particles, apply curvature-based force
- `F_tension = -σ * κ * n` where σ is tension coefficient, κ is curvature
- Computationally expensive (requires second derivatives of kernel)
- Can cause instability if too strong

**Dependencies**: Kernel second derivatives, normal/curvature estimation

**Trade-off**: Adds visual polish but impacts performance and stability. May skip for initial implementation.

**References**:
- [Versatile Surface Tension and Adhesion for SPH Fluids](https://dl.acm.org/doi/10.1145/2508363.2508395)
- [Implicit Surface Tension for SPH Fluid Simulation](https://dl.acm.org/doi/10.1145/3631936)

---

### 10. Interactive Parameter Tuning
**Description**: Runtime UI to adjust viscosity, stiffness, surface tension, gravity during simulation. Sliders or keyboard controls.

**Visual Impact**: Enables live experimentation and finding sweet spot for visual quality. Great for demos and tweaking.

**Complexity**: Trivial
- Expose constants as runtime variables
- Add keyboard handlers to increment/decrement
- Print current values to screen

**Dependencies**: Input system (already exists)

---

### 11. Interactive Obstacle Placement
**Description**: Click to place solid obstacles (circles, boxes) that fluid flows around.

**Visual Impact**: Makes simulation feel interactive and playful. Watching fluid flow around user-placed objects is engaging.

**Complexity**: Moderate
- Extend boundary handling to dynamic obstacles
- Need collision detection between particles and obstacle geometry
- Can use boundary particles or penalty forces

**Dependencies**: Boundary handling, input system

---

### 12. Color Field / Particle Coloring
**Description**: Color particles by velocity magnitude, pressure, or density. Visualizes internal fluid state.

**Visual Impact**: Helps debugging and looks scientific/impressive. High-pressure regions in red, low in blue, etc.

**Complexity**: Trivial
- Map scalar value to color gradient
- Already supported by Particle2D color field

**Dependencies**: None

---

### 13. Multiple Fluid Phases
**Description**: Simulate two fluids with different properties (e.g., water and oil) that don't mix.

**Visual Impact**: Visually striking to see immiscible fluids interact. Adds complexity and visual interest.

**Complexity**: Complex
- Requires phase tracking per particle
- Modified pressure/density calculations for multi-phase
- Surface tension between phases

**Dependencies**: All core SPH features

**Recommendation**: Out of scope for initial milestone, but impressive future addition

---

### 14. Splashing/Spray Particles
**Description**: Detect high-velocity surface particles and render as spray/mist.

**Visual Impact**: Adds realism to violent fluid motion (dam break impact, waves). Small particles look like spray.

**Complexity**: Moderate
- Detect surface particles with velocity above threshold
- Render smaller or with different shader
- Can transition to ballistic (non-SPH) particles for performance

**Dependencies**: Surface detection

---

## Anti-Features
*Things that hurt visual quality or performance*

### A1. Tensile Instability / Particle Clumping
**Description**: SPH artifact where particles under tension (negative pressure) form unrealistic pairs or clusters instead of flowing smoothly. Caused by kernel gradient behavior at medium distances.

**Visual Damage**: Fluid breaks into clumps or forms regular lattice patterns instead of smooth flow. Looks artificial and broken.

**Mitigation**:
- Use **artificial stress** technique: add small repulsive force when tension detected
- Choose kernels carefully (Spiky gradient for pressure prevents some clumping)
- Ensure smoothing length h is 2-3× particle spacing
- Adaptive kernel approaches (complex)

**Complexity**: Moderate mitigation effort

**References**:
- [Tensile Instability and Artificial Stresses in SPH](https://www.researchgate.net/publication/254496153_Tensile_Instability_and_Artificial_Stresses_in_Impact_Problems_in_SPH)
- [An Adaptive Approach to Remove Tensile Instability](https://www.sciencedirect.com/science/article/abs/pii/S0045793023003353)

---

### A2. Excessive Stiffness (Timestep Collapse)
**Description**: WCSPH uses stiff equations of state. Too-high stiffness constant makes pressure forces enormous, requiring tiny timesteps to remain stable.

**Visual Damage**: Simulation slows to crawl (1-2 fps) or explodes (particles fly off screen). Makes real-time interaction impossible.

**Mitigation**:
- Use moderate stiffness (k = 1000-3000 for visual plausibility)
- Accept ~1% density variation instead of perfect incompressibility
- Clamp velocity or acceleration to prevent explosions
- Use larger timesteps with lower stiffness (trade accuracy for speed)

**Complexity**: Trivial to tune, requires experimentation

**Performance Impact**: Stiffness directly trades off stability vs compressibility

---

### A3. Particle Deficiency at Boundaries
**Description**: Particles near walls or free surface have fewer neighbors, causing inaccurate density calculations and anomalous forces.

**Visual Damage**:
- Particles stick to walls
- Surface particles have wrong pressure (can fly off or sink)
- Uneven density field near boundaries

**Mitigation**:
- Boundary particles provide missing density contribution
- Ghost particles (mirror across boundary)
- Normalization corrections for kernel summation
- Sufficient boundary particle layers (2-3 rows)

**Complexity**: Moderate (proper boundary handling is hard)

**Note**: Known difficult problem in SPH - "particle deficiency issues" invalidate standard methods

---

### A4. Over-Damping from Excessive Viscosity
**Description**: Too much viscosity makes fluid sluggish, loses energy quickly, settles into static state.

**Visual Damage**: Fluid moves like honey or gel instead of water. Dam break lacks impact and splash.

**Mitigation**:
- Use low viscosity for water (~0.001 - 0.01)
- Tune to taste during interactive testing
- Consider separate kinematic viscosity for surface smoothness

**Complexity**: Trivial (parameter tuning)

---

### A5. Poor Neighbor Search Performance
**Description**: Using brute-force O(n²) neighbor search or inefficient spatial structure.

**Visual Damage**: Frame rate drops below 20fps, simulation feels unresponsive.

**Mitigation**:
- Implement uniform grid hashing (O(n) for uniform density)
- Octree may be overkill but exists in codebase
- Profile and optimize the hottest loop

**Complexity**: Moderate implementation, critical for performance

**Performance**: Neighbor search is typically 40-60% of CPU time in SPH

**Reference**: [SPH and CPU Multi-threading](https://medium.com/@ahmed.fathy.elbossily/sph-and-cpu-multi-threading-00855c9cfe82)

---

### A6. Rendering Every Particle
**Description**: Drawing 5,000 individual circles every frame with CPU-based SDL2 rendering.

**Visual Damage**: Frame rate drops due to rendering, not physics. Simulation runs fast but display is slow.

**Mitigation**:
- Batch rendering calls
- Skip rendering particles outside viewport
- Reduce particle visual radius
- If using metaballs, evaluate field on coarser grid
- Consider OpenGL for hardware-accelerated rendering (breaks SDL2-only constraint)

**Complexity**: Trivial optimizations, moderate for HW acceleration

---

### A7. Uniform Particle Distribution Enforcement
**Description**: Some implementations actively try to maintain perfect particle spacing, fighting against natural density variations.

**Visual Damage**: Fluid looks overly regular, loses natural chaotic variation. Can cause artificial spreading or "popping" artifacts.

**Mitigation**: Don't implement this. Let WCSPH pressure naturally regulate density without geometric constraints.

**Complexity**: N/A (anti-pattern to avoid)

---

### A8. Ignoring CFL Condition
**Description**: Using fixed large timestep without checking Courant–Friedrichs–Lewy stability condition: `Δt ≤ λ * h / v_max` (λ ≈ 0.25).

**Visual Damage**: Particles tunnel through boundaries, simulation explodes, or exhibits strange artifacts.

**Mitigation**:
- Implement adaptive timestepping based on max velocity
- Or use very conservative fixed timestep (0.001s)
- Monitor max velocity and reduce timestep if needed

**Complexity**: Trivial

---

### A9. Single Particle Radius for All Scenarios
**Description**: Choosing particle radius too large (overlapping densities, blobby) or too small (gaps in fluid, noisy).

**Visual Damage**: Either overdense (high pressure, explosions) or underdense (gaps, lack of cohesion).

**Mitigation**:
- Smoothing length h should be 2-3× initial particle spacing
- For 5,000 particles in reasonable domain, tune h to give ~20-50 neighbors per particle
- Test with different particle counts and adjust

**Complexity**: Trivial (parameter tuning)

---

## Implementation Priority for Dam Break Showcase

### Phase 1: Core Fluid (Table Stakes)
1. SPH kernel functions (Poly6, Spiky, Viscosity)
2. Neighbor search optimization (adapt Octree2D or grid hash)
3. WCSPH pressure solver with density calculation
4. Viscosity forces
5. Boundary particles for container walls
6. Gravity + time integration (already exists)

**Goal**: Fluid that flows and looks like liquid (even if rendered as circles)

---

### Phase 2: Visual Polish (Differentiators)
7. Metaball rendering with marching squares
8. Interactive parameter tuning (sliders for viscosity, stiffness)
9. Color by pressure/velocity
10. Performance profiling and optimization

**Goal**: Impressive visual presentation and interactivity

---

### Phase 3: Advanced (Optional)
11. Surface tension
12. Interactive obstacle placement
13. Splashing particles
14. Multi-phase fluids

**Goal**: Showcase features, research extensions

---

## Performance Budget

Target: **5,000 particles at 60fps** (16.67ms per frame)

Estimated time distribution:
- Neighbor search: ~6ms (40%)
- Density + pressure: ~3ms (20%)
- Forces (pressure + viscosity): ~4ms (25%)
- Integration: ~1ms (5%)
- Rendering: ~2ms (10%)

**Critical path**: Neighbor search must be O(n) amortized, not O(n²)

---

## Sources

Research based on:

- [Journey into SPH Simulation Framework](https://arxiv.org/html/2403.11156v1)
- [SPlisHSPlasH Features](https://splishsplash.physics-simulation.org/features/)
- [Particle-Based Fluid Simulation for Interactive Applications (Müller et al. 2003)](https://matthias-research.github.io/pages/publications/sca03.pdf)
- [Surface Reconstruction of SPH Fluids](https://ramakarl.com/surface-reconstruction-of-sph-fluids/)
- [Tensile Instability in SPH](https://www.researchgate.net/publication/254496153_Tensile_Instability_and_Artificial_Stresses_in_Impact_Problems_in_SPH)
- [An Adaptive Approach to Remove Tensile Instability](https://www.sciencedirect.com/science/article/abs/pii/S0045793023003353)
- [Versatile Surface Tension and Adhesion for SPH](https://dl.acm.org/doi/10.1145/2508363.2508395)
- [SPH and CPU Multi-threading](https://medium.com/@ahmed.fathy.elbossily/sph-and-cpu-multi-threading-00855c9cfe82)
- [Modeling Dam Break Flows Using SPH](https://www.iahr.org/library/infor?pid=24717)

---

*Generated: 2026-01-23 for PhysicsEngine WCSPH milestone*
