# SPH Implementation Stack for WCSPH Fluid Simulation

## Executive Summary

This document provides prescriptive implementation guidance for adding Weakly Compressible SPH (WCSPH) to the existing particle physics engine. All recommendations are optimized for CPU-based real-time visual simulation (1,000-5,000 particles at 60fps) with SDL2 rendering and future 3D extensibility.

---

## 1. Kernel Functions

### Recommended: Wendland C2 Kernel

**Why Wendland C2:**
- Compact support (zero beyond smoothing radius h) enables efficient neighbor search
- C² continuous (smooth second derivative) prevents numerical instabilities
- Lower computational cost than higher-order kernels (C4, C6)
- Optimal balance between smoothness and performance for visual simulation
- Well-proven in modern SPH implementations (Dehnen & Aly 2012)

### Formulas

**2D Wendland C2:**
```
q = r/h  (normalized distance)

W(r,h) = α_d × (1-q)⁴ × (4q + 1)   for 0 ≤ q < 1
       = 0                          for q ≥ 1

Normalization constant α_d:
  2D: α_d = 7/(4πh²)
  3D: α_d = 21/(2πh³)
```

**Gradient (derivative w.r.t. position):**
```
∇W(r,h) = α_d × (-20q³/h) × (r/|r|)   for 0 ≤ q < 1
        = 0                            for q ≥ 1
```

**Implementation Notes:**
- Pre-compute α_d × h^(-d) where d is dimension
- Store (1-q)³ to avoid redundant computation in kernel and gradient
- The gradient direction is always along r_ab = r_a - r_b

### Alternative: Cubic Spline (Fallback Option)

**Use only if:** Wendland C2 produces insufficient viscosity for your visual target.

**2D Cubic Spline:**
```
q = r/h

W(r,h) = α_d × [(2-q)³ - 4(1-q)³]   for 0 ≤ q < 1
       = α_d × (2-q)³                for 1 ≤ q < 2
       = 0                           for q ≥ 2

Normalization:
  2D: α_d = 10/(7πh²)
  3D: α_d = 1/(πh³)
```

**Why NOT cubic spline by default:**
- Support radius 2h requires 4× more neighbors in 2D, 8× in 3D
- Piecewise linear second derivative causes stability issues
- Significantly higher computational cost for marginal visual benefit

**DO NOT use Gaussian kernels** - they have infinite support and defeat the purpose of spatial partitioning.

---

## 2. Neighbor Search Strategy

### Recommended: Extend Existing Octree2D with Radius Query

**Implementation Approach:**
You already have `Octree2D` for broad-phase collision detection. Extend it with:

```cpp
std::vector<Particle2D*> queryRadius(const Vec2& center, float radius);
```

**Why leverage existing Octree:**
- Already implemented and debugged
- Natural extension of current architecture
- Octree adapts well to non-uniform particle distributions (splashes, waves)
- Recent research shows octrees competitive with uniform grids for SPH (Longva et al. 2022)

**Performance Optimization:**
- Set octree cell size ≈ smoothing length h
- Rebuild octree every frame (particles move significantly in fluids)
- Pre-allocate neighbor list vectors with capacity ~32 for 2D, ~100 for 3D

**Smoothing Length Guideline:**
- Set h = 1.2 to 1.5 × particle_spacing
- Target ~30-50 neighbors per particle in 2D
- Too small h → particle isolation, pressure instability
- Too large h → over-smoothing, loss of surface detail

**Alternative if Octree Performance Insufficient:**
Implement **Spatial Hashing** (uniform grid with hashmap):
- Cell size = h (smoothing length)
- Hash function: `hash(x,y) = (x*73856093) ^ (y*19349663)`
- Check 9 cells in 2D (3×3 grid), 27 in 3D
- Faster construction than octree for uniform distributions
- Simpler implementation: ~50 lines of code

**DO NOT** iterate all particle pairs (O(n²)) - this will never reach 60fps beyond 100 particles.

---

## 3. Integration Scheme

### Recommended: Symplectic Euler (Predictor-Corrector)

**Why Symplectic Euler for WCSPH:**
- **Time-reversible and energy-conserving** for Lagrangian schemes
- Second-order accurate (better than Euler, same order as Verlet/RK2)
- **Lower computational cost than RK4** (1 force evaluation per step vs 4)
- Well-matched to WCSPH pressure solver characteristics
- Used successfully in production SPH codes (DualSPHysics, SPlisHSPlasH)

**Algorithm (Predictor-Corrector Mode):**

```cpp
// Predictor: advance to mid-timestep
v_predict[i] = v[i] + 0.5 * dt * acceleration[i];
x_predict[i] = x[i] + 0.5 * dt * v_predict[i];

// Compute forces at predicted positions
update_density(x_predict);      // SPH density summation
compute_pressure();              // Equation of state
acceleration_new = compute_sph_forces(x_predict, v_predict);

// Corrector: full timestep with new forces
v[i] = v[i] + dt * acceleration_new[i];
x[i] = x[i] + dt * v[i];
```

**Integration with Existing System:**
- Your engine already has RK4, Euler, Verlet options
- Add `INTEGRATION_SYMPLECTIC_EULER` as new enum value
- Reuse existing force accumulation infrastructure
- Keep RK4 for rigid body dynamics, use Symplectic Euler for fluids

**Why NOT RK4 for fluids:**
- 4× more neighbor searches per timestep (very expensive for SPH)
- Excessive for WCSPH accuracy requirements
- Your target is visual plausibility, not scientific precision
- Better to spend computation on more particles than higher-order integration

### Time Step Calculation (Critical for Stability)

**CFL Condition for WCSPH:**
```cpp
float dt_cfl = CFL_NUMBER * h / (c_sound + v_max);
float dt_force = sqrt(h / a_max);
float dt_viscosity = h * h / (2.0f * viscosity_kinematic);

dt = safety_factor * min(dt_cfl, dt_force, dt_viscosity);
```

**Recommended Values:**
- `CFL_NUMBER = 0.25` (conservative, increase to 0.4 if stable)
- `safety_factor = 0.4` (accounts for all three constraints)
- Re-compute `dt` every frame based on current max velocity/acceleration

**The CFL condition dominates** for WCSPH - you'll typically be limited by speed of sound.

---

## 4. WCSPH Parameters

### Equation of State (Tait Equation)

**Purpose:** Convert density to pressure in weakly-compressible approach.

**Formula:**
```cpp
float B = (c_sound * c_sound * rest_density) / gamma;
float pressure = B * (pow(density/rest_density, gamma) - 1.0f);
```

**Parameter Values (Water-like Visual Behavior):**

| Parameter | Value | Rationale |
|-----------|-------|-----------|
| `rest_density` | 1000 kg/m³ | Physical water density (unchanging reference) |
| `gamma` | 7 | Standard for water (isentropic exponent) |
| `c_sound` | 20 × √(g×H) | Artificial speed of sound |
| `particle_mass` | rest_density × (spacing)² | Maintains uniform density at rest |

**Speed of Sound Calculation:**
```cpp
// H = characteristic height (e.g., dam height in meters)
// g = 9.81 m/s² (gravity)
float c_sound = 20.0f * sqrt(9.81f * H);

// Example: For 2m tall dam break
// c_sound = 20 × sqrt(9.81 × 2) ≈ 88.6 m/s
```

**Why c = 20√(gH):**
- Limits density variations to ~1% (ensures weak compressibility)
- Smaller c → larger time steps → better performance
- Larger c → stiffer pressure forces → smaller time steps required
- 10√(gH) is minimum stable, 20√(gH) is sweet spot, 50√(gH) approaches incompressible

### Viscosity

**Artificial Viscosity (Monaghan 1992):**
```cpp
float alpha = 0.01f to 0.1f;  // Start at 0.02
float beta = 0.0f;             // Usually zero for fluids

// In momentum equation:
if (v_ab · r_ab < 0) {  // Only for approaching particles
    float mu = h * (v_ab · r_ab) / (r_ab² + 0.01*h²);
    float PI_ab = (-alpha * c_sound * mu + beta * mu²) / rho_avg;
    acceleration -= mass_b * PI_ab * gradW_ab;
}
```

**Visual Tuning:**
- `alpha = 0.01`: Water-like (splashy, thin)
- `alpha = 0.05`: Honey-like (viscous, smooth)
- `alpha = 0.1`: Extremely viscous (slow motion)

**DO NOT** set alpha > 0.1 - produces excessive damping and unnatural motion.

### Boundary Conditions

**Recommended: Dynamic Boundary Particles**

Create boundary particles that:
1. Contribute to density summation (prevent under-density at walls)
2. Exert pressure forces (prevent penetration)
3. Do not move (fixed positions)

**Implementation:**
```cpp
// For each fluid particle near boundary:
for (boundary_particle : boundary_neighbors) {
    // Boundary contributes to density
    density += mass_boundary * W(r_ab, h);

    // Boundary exerts pressure force (symmetric)
    float pressure_force = mass_boundary * (P_a/rho_a² + P_b/rho_b²);
    acceleration -= pressure_force * gradW_ab;
}
```

**Spacing:** Place boundary particles at same spacing as fluid particles.

**Alternative (simpler but less robust):** Penalty force when particle approaches wall.

---

## 5. Numerical Stability Enhancements

### Density Diffusion (δ-SPH) - ESSENTIAL

**Problem:** WCSPH suffers from numerical density noise → pressure noise → jittery motion.

**Solution:** Delta-SPH density diffusion term (Antuono et al. 2010)

**Add to continuity equation:**
```cpp
// Standard density rate:
float drho_dt = sum over neighbors: mass_b * (v_ab · gradW_ab);

// Add diffusion term:
float delta = 0.1f;  // diffusion coefficient
float psi_ab = 2.0f * (rho_a - rho_b) * (r_ab / r_ab.lengthSquared());
drho_dt += delta * h * c_sound * sum over neighbors: mass_b * psi_ab * gradW_ab;
```

**Recommended delta values:**
- Start with `delta = 0.1`
- Increase to 0.2 if pressure oscillations visible
- Values above 0.5 cause excessive artificial smoothing

**Visual Impact:** Transforms jittery, noisy simulation into smooth, stable fluid motion.

### Tensile Instability Prevention

**Problem:** Negative pressure causes particle clumping and void formation.

**Solution 1: Background Pressure (Simplest)**
```cpp
// Add constant positive offset to all pressures
float P_background = 0.1f * B;  // 10% of bulk modulus
pressure = max(0.0f, tait_pressure + P_background);
```

**Solution 2: Particle Shifting (Better Visual Quality)**
Adjust particle positions slightly each frame to maintain regular distribution:
```cpp
// Shift particles toward regions of higher concentration
Vec2 shift = -0.01 * h * sum over neighbors: (r_ab / |r_ab|) * W(r_ab, h);
position += shift;
```

**Recommendation:** Start with background pressure (2 lines of code). Add particle shifting if you observe:
- Particles clumping into strings or clusters
- Unnatural holes/voids in fluid bulk
- Unrealistic fragmentation of jets

**DO NOT** use artificial stress tensors - too complex, marginal benefit for visual simulation.

---

## 6. What to Avoid (Common Mistakes)

### ❌ Using RK4 Integration
- **Why it fails:** 4× cost, minimal visual improvement, breaks through CFL limit faster
- **Do instead:** Symplectic Euler with adaptive timestep

### ❌ Skipping Density Diffusion
- **Why it fails:** Pressure noise makes simulation look unrealistic and jittery
- **Do instead:** Always implement δ-SPH with delta ≥ 0.1

### ❌ Setting Speed of Sound Too Low
- **Why it fails:** c < 10√(gH) causes >10% density variations, violates weak compressibility
- **Impact:** Particles penetrate each other, fluid looks "gassy"
- **Do instead:** Use c = 20√(gH) minimum

### ❌ Fixed Time Step
- **Why it fails:** Simulation explodes when particles move faster than expected
- **Do instead:** Adaptive time step based on CFL condition every frame

### ❌ All-Pairs Neighbor Search
- **Why it fails:** O(n²) scaling, unusable beyond 100 particles
- **Do instead:** Octree radius query or spatial hashing (O(n log n) or O(n))

### ❌ Forgetting Boundary Particles
- **Why it fails:** Fluid penetrates walls, particles "leak" through geometry
- **Do instead:** Create layer of fixed boundary particles at walls

### ❌ Too Many Neighbors
- **Why it fails:** Smoothing length h > 2× particle spacing over-smooths detail
- **Impact:** Fluid looks blobby, surface features disappear
- **Do instead:** h = 1.2 to 1.5× particle spacing

### ❌ Pressure from All Particles
- **Why it fails:** Neglecting particle i's own contribution in pressure summation
- **Do instead:** Symmetric pressure force: `P_a/rho_a² + P_b/rho_b²`

### ❌ Using Eulerian Density
- **Why it fails:** Summing density from neighbors doesn't conserve mass
- **Do instead:** Use continuity equation (Lagrangian approach) with density diffusion

### ❌ Ignoring Viscosity
- **Why it fails:** Zero viscosity simulation produces chaotic, uncontrollable motion
- **Do instead:** Start with alpha = 0.02, tune for desired visual "thickness"

---

## 7. Implementation Roadmap

### Phase 1: Core SPH Machinery (Week 1)
1. Extend `Octree2D::queryRadius()` for neighbor search
2. Add Wendland C2 kernel functions (W and ∇W)
3. Implement density summation loop
4. Implement Tait equation of state

**Validation:** Static particle grid should show uniform density ≈ rest_density.

### Phase 2: Forces and Integration (Week 1-2)
1. Implement pressure force calculation
2. Add artificial viscosity
3. Implement symplectic Euler integrator
4. Add adaptive time stepping (CFL condition)

**Validation:** Dropping a particle "blob" should fall and splat realistically.

### Phase 3: Boundaries (Week 2)
1. Create boundary particle generator for rectangular walls
2. Extend neighbor search to include boundary particles
3. Implement boundary pressure forces

**Validation:** Fluid in box should not leak through walls.

### Phase 4: Stability (Week 2-3)
1. Implement δ-SPH density diffusion
2. Add background pressure
3. Tune all parameters for visual quality

**Validation:** Dam break simulation runs stable for 10+ seconds without explosion.

### Phase 5: Optimization (Week 3+)
1. Profile neighbor search performance
2. Optimize memory layout (Structure of Arrays)
3. Consider SIMD vectorization for force loops
4. Pre-compute kernel normalizations

**Target:** 5,000 particles at 60fps on modern CPU.

---

## 8. Recommended Starting Parameters (Dam Break Test)

```cpp
// Simulation domain: 10m × 5m
// Dam geometry: 2m × 1m water column on left side

const float PARTICLE_SPACING = 0.02f;    // 2cm particles
const float H = 1.5f * PARTICLE_SPACING; // Smoothing length
const float REST_DENSITY = 1000.0f;      // kg/m³
const float GAMMA = 7.0f;                // Dimensionless
const float DAM_HEIGHT = 1.0f;           // meters

// Derived parameters
const float C_SOUND = 20.0f * sqrt(9.81f * DAM_HEIGHT);  // ≈ 62.7 m/s
const float B = (C_SOUND * C_SOUND * REST_DENSITY) / GAMMA;
const float PARTICLE_MASS = REST_DENSITY * PARTICLE_SPACING * PARTICLE_SPACING;

// Viscosity and diffusion
const float ALPHA_VISCOSITY = 0.02f;
const float DELTA_DIFFUSION = 0.1f;

// Time stepping
const float CFL_NUMBER = 0.25f;
const float SAFETY_FACTOR = 0.4f;

// Expected particle count: 1m × 2m / (0.02m)² = 5,000 particles
```

**Performance Estimate:** ~2-3ms per frame on modern CPU with optimized octree (333-500fps).

---

## 9. References and Further Reading

### Academic Papers
- Wendland kernels: [Dehnen & Aly (2012)](https://arxiv.org/pdf/2104.09016) - Optimized smoothing kernels
- Octree neighbor search: [Longva et al. (2022)](https://andreaslongva.com/pdf/2022-SA-NeighborhoodSearch-compressed.pdf) - Fast octree implementation
- Symplectic integration: [DualSPHysics Forums](https://forums.dual.sphysics.org/discussion/1770/time-stepping-formulas-velocity-prediction-with-sympleptic-scheme)
- WCSPH overview: [Interactive Computer Graphics](https://interactivecomputergraphics.github.io/physics-simulation/examples/wcsph.html)
- Density diffusion: [Antuono et al. (2010) δ-SPH](https://github.com/DualSPHysics/DualSPHysics/wiki/3.-SPH-formulation)
- Tensile instability solutions: [Monaghan (2000) artificial stress](https://www.mdpi.com/2073-4441/12/11/3189)
- CFL condition for SPH: [DualSPHysics Time Step](https://forums.dual.sphysics.org/discussion/1808/theory-time-step-size)

### Software Libraries (Reference Only - Not Dependencies)
- **SPlisHSPlasH**: Open-source C++ SPH library (excellent reference implementation)
- **DualSPHysics**: Production WCSPH code (GPU and CPU versions)
- **PySPH**: Python SPH with comprehensive equation catalog

### Implementation Guides
- [GPUSPH Theory Guide](https://www.gpusph.org/documentation/gpusph-theory.pdf) - Comprehensive formulas
- [SPH Tutorial by Kelager (2006)](https://pmocz.github.io/manuscripts/pmocz_sph.pdf) - Beginner-friendly
- [Physics-Based Animation](https://www.physicsbasedanimation.com/) - Visual simulation focus

---

## 10. Success Criteria

### Minimum Viable Product (MVP)
- [ ] 2D dam break runs for 10 seconds without crash
- [ ] Fluid stays contained within boundaries
- [ ] No visible particle clumping or voids
- [ ] Performance: 1,000 particles at 60fps minimum

### Production Quality
- [ ] Smooth, jitter-free motion (enabled by δ-SPH)
- [ ] Realistic splashing and wave formation
- [ ] Stable under parameter variations (±20%)
- [ ] Performance: 5,000 particles at 60fps target

### 3D Extension Ready
- [ ] All formulas have 3D versions documented
- [ ] Octree already supports 3D (Octree3D exists or trivial to add)
- [ ] Kernel normalization constants parameterized by dimension
- [ ] No hardcoded 2D assumptions in core algorithms

---

## Notes

This stack represents the **2025 state-of-the-art for real-time WCSPH** suitable for visual simulation. All recommendations prioritize:

1. **Simplicity:** Minimum complexity to achieve stable, attractive fluid motion
2. **Performance:** CPU-achievable 60fps with thousands of particles
3. **Robustness:** Forgiving of parameter variations, unlikely to explode
4. **Extensibility:** Clear path to 3D without architectural changes

**When in doubt, implement the simplest option first.** You can always add sophistication later - but you must establish a stable baseline before optimization.

---

**Document Version:** 1.0
**Last Updated:** 2026-01-23
**Target Milestone:** WCSPH Fluid Simulation Integration
