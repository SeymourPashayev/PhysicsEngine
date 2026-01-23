# SPH Architecture

## Integration with Existing Engine

### Layer Structure
WCSPH integrates into the existing 5-layer architecture:

```
Application Layer (Main.cpp, Application.cpp)
    ├─ Game Loop: Input → Update → Render
    └─ Scene Management
        ↓
System Layer (System.hpp/cpp)
    ├─ Particle collection (std::vector<Particle2D*>)
    ├─ Physics toggles (FLUID_DYNAMICS_ENABLED, etc.)
    └─ Advect() / Project() / Draw()
        ↓
Scene Layer (WaterDam, SpringForceLattice)
    └─ Concrete simulation setups
        ↓
Physics Layer ← **SPH SOLVER LIVES HERE**
    ├─ Particle2D/3D (data carriers)
    ├─ SPHSolver (new: orchestrates SPH pipeline)
    ├─ SPHKernel (existing: kernel functions)
    ├─ Force (existing: force generators)
    ├─ Octree2D (existing: spatial queries)
    └─ Vec2/Vec3 (existing: math)
        ↓
Graphics Layer (Graphics.hpp, SDL2)
    └─ Rendering (particles, metaballs)
```

### Integration Points

**Primary Integration: System::Advect()**
```cpp
// Current structure in System.cpp
void System::Advect(float dt, Vec2 pushForce) {
    CalculateSmoothingLength();      // ← Already doing SPH prep
    CheckForScreenCollisions();
    CheckForParticleCollisions();
    CreateParticleAtMouse();

    // Force accumulation loop
    for (auto particle: particles) {
        particle->AddForce(pushForce);
        if (GRAVITY_ENABLED) particle->AddForce(Force::GenerateWeightForce(*particle));
        if (FRICTION_ENABLED) particle->AddForce(Force::GenerateFrictionForce(*particle, 0.05f));
        if (ATTRACTION_ENABLED) AttractionForceCalculatorHelper(*particle);
        if (DRAG_ENABLED) particle->AddForce(Force::GenerateDragForce(*particle, 0.09f));
        if (INTEGRATION_ENABLED) particle->RK4Integrate(dt);
    }
}
```

**Proposed SPH Integration:**
```cpp
void System::Advect(float dt, Vec2 pushForce) {
    // 1. Spatial structure update
    octree->rebuild(particles);  // or Clear() + re-insert all

    // 2. SPH pipeline (when FLUID_DYNAMICS_ENABLED)
    if (FLUID_DYNAMICS_ENABLED) {
        sphSolver->ComputeDensityPressure(particles, octree);
        sphSolver->ComputeForces(particles, octree);
    }

    // 3. Other forces (existing)
    for (auto particle: particles) {
        if (GRAVITY_ENABLED) particle->AddForce(Force::GenerateWeightForce(*particle));
        // ... other forces
    }

    // 4. Boundary conditions
    if (SCREEN_COLLISION_ENABLED) CheckForScreenCollisions();

    // 5. Integration
    if (INTEGRATION_ENABLED) {
        for (auto particle: particles) {
            particle->RK4Integrate(dt);
        }
    }
}
```

**Key Decision: SPHSolver as Standalone Component**
- Not a subclass of System
- Not inside Particle2D (keeps particles as data carriers)
- Physics layer utility class, similar to Force
- System owns an `SPHSolver* sphSolver` instance
- Advantages:
  - Clean separation: particles = data, solver = algorithms
  - Easier to swap solvers (WCSPH → PCISPH later)
  - Testable in isolation
  - Reusable across different System subclasses

---

## Data Organization

### SPH-Specific Data Locations

#### 1. Per-Particle State (Particle2D.hpp - ALREADY EXISTS)
```cpp
struct Particle2D {
    // Existing mechanics
    Vec2 position, velocity, acceleration;
    Vec2 sumForces;
    float mass, invMass, radius;

    // SPH properties (ALREADY STUBBED)
    float smoothingLength;    // Per-particle or global?
    float density;            // ρ (rho)
    float pressure;           // P
    Vec2 pressureForce;       // F_pressure
    Vec2 viscosityForce;      // F_viscosity

    // SPH computation methods (ALREADY DECLARED)
    void ComputeDensity(const std::vector<Particle2D*>& neighbors);
    void ComputePressure();
    void ComputePressureForce(const std::vector<Particle2D*>& neighbors);
    void ComputeViscosityForce(const std::vector<Particle2D*>& neighbors);
};
```

**Note:** Particle methods already exist but may delegate to SPHSolver. Options:
- **Option A (Recommended):** Particle methods are thin wrappers that call SPHSolver
  - `particle->ComputeDensity(neighbors)` → `SPHSolver::ComputeDensity(particle, neighbors)`
  - Keeps existing API, internal refactor only
- **Option B:** Deprecate particle methods, call SPHSolver directly from System
  - Cleaner but breaks existing API pattern

#### 2. Solver State (SPHSolver class - NEW)
```cpp
class SPHSolver {
private:
    // Simulation parameters
    float restDensity;        // ρ₀ (from Constants.hpp)
    float stiffness;          // k (gas constant)
    float viscosity;          // μ (viscosity coefficient)
    float smoothingLength;    // h (kernel radius)

    // Kernel reference
    SPHKernel* kernel;        // Poly6, Spiky, Viscosity kernels

public:
    SPHSolver(float h, float rho0, float k, float mu);

    // Pipeline stages (see Update Pipeline section)
    void ComputeDensityPressure(std::vector<Particle2D*>& particles, Octree2D* octree);
    void ComputeForces(std::vector<Particle2D*>& particles, Octree2D* octree);

    // Parameter tuning (for runtime adjustment)
    void SetViscosity(float mu);
    void SetStiffness(float k);
    void SetSmoothingLength(float h);
};
```

#### 3. Global Constants (Constants.hpp - ALREADY EXISTS)
```cpp
// Already defined:
const float REST_DENSITY = 1000.0f;   // kg/m³ (water)
const float STIFFNESS = 100.0f;       // Gas constant k
const float VISCOSITY = 0.5f;         // Viscosity coefficient μ
```

**Missing:** Need to add SPH-specific constants
```cpp
const float SPH_SMOOTHING_LENGTH = 16.0f;  // in pixels (kernel radius h)
const float SPH_PARTICLE_MASS = 2.0f;      // Mass per fluid particle
```

#### 4. Kernel Functions (SPHKernel - ALREADY EXISTS)
```cpp
// Already implemented in SPHKernel.cpp
class SPHKernel {
    static float h;  // smoothing length

    static float Poly6(float r);                    // Density: W(r)
    static Vec2 SpikyGradient(const Vec2& r);       // Pressure: ∇W(r)
    static float ViscosityLaplacian(float r);       // Viscosity: ∇²W(r)
};
```

**Status:** Implementation complete, just needs initialization in SPHSolver constructor.

---

## Update Pipeline

### SPH Algorithm Execution Order

The WCSPH algorithm requires strict ordering due to data dependencies:

```
┌─────────────────────────────────────────────────────────────┐
│ 1. SPATIAL STRUCTURE UPDATE                                 │
│    octree->rebuild(particles)                               │
│    Purpose: Accelerate neighbor queries in steps 2-3        │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│ 2. DENSITY & PRESSURE COMPUTATION                           │
│    For each particle i:                                     │
│      a) Find neighbors within smoothing radius h            │
│         neighbors = octree->findNeighbors(i, h)             │
│      b) Compute density ρᵢ                                  │
│         ρᵢ = Σⱼ mⱼ * W(rᵢⱼ, h)  [Poly6 kernel]              │
│      c) Compute pressure Pᵢ                                 │
│         Pᵢ = k * (ρᵢ - ρ₀)     [Tait equation]              │
│                                                             │
│    Output: particle->density, particle->pressure populated  │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│ 3. FORCE COMPUTATION                                        │
│    For each particle i:                                     │
│      a) Find neighbors (reuse from step 2 if cached)        │
│      b) Compute pressure force                              │
│         F_pressure = -Σⱼ mⱼ * (Pᵢ+Pⱼ)/(2ρⱼ) * ∇W(rᵢⱼ)       │
│         [Spiky gradient kernel]                             │
│      c) Compute viscosity force                             │
│         F_viscosity = μ * Σⱼ mⱼ * (vⱼ-vᵢ)/ρⱼ * ∇²W(rᵢⱼ)     │
│         [Viscosity Laplacian kernel]                        │
│      d) Add to particle force accumulator                   │
│         particle->AddForce(F_pressure + F_viscosity)        │
│                                                             │
│    Output: Forces added to particle->sumForces              │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│ 4. EXTERNAL FORCES (existing system)                        │
│    if (GRAVITY_ENABLED): AddForce(gravity)                  │
│    if (FRICTION_ENABLED): AddForce(friction)                │
│    etc.                                                     │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│ 5. BOUNDARY CONDITIONS                                      │
│    CheckForScreenCollisions()  [existing]                   │
│    CheckForParticleCollisions() [may disable for fluids]    │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│ 6. INTEGRATION                                              │
│    For each particle:                                       │
│      particle->RK4Integrate(dt)                             │
│    Updates position/velocity from accumulated forces        │
└─────────────────────────────────────────────────────────────┘
```

### Implementation in System::Advect()

```cpp
void System::Advect(float dt, Vec2 pushForce) {
    // ── STEP 1: Rebuild spatial structure ──
    octree->rebuild(particles);

    // ── STEP 2 & 3: SPH density, pressure, forces ──
    if (FLUID_DYNAMICS_ENABLED) {
        // Step 2: Density & Pressure (MUST happen first - needed for forces)
        sphSolver->ComputeDensityPressure(particles, octree);

        // Step 3: SPH Forces (depends on density/pressure from step 2)
        sphSolver->ComputeForces(particles, octree);
    }

    // ── STEP 4: External forces ──
    for (auto particle: particles) {
        if (KEYBOARD_ENABLED) particle->AddForce(pushForce);
        if (GRAVITY_ENABLED) particle->AddForce(Force::GenerateWeightForce(*particle));
        if (FRICTION_ENABLED) particle->AddForce(Force::GenerateFrictionForce(*particle, 0.05f));
        // Note: ATTRACTION_ENABLED probably disable for fluids (particle-particle gravity)
        if (DRAG_ENABLED) particle->AddForce(Force::GenerateDragForce(*particle, 0.09f));
    }

    // ── STEP 5: Boundary conditions ──
    if (SCREEN_COLLISION_ENABLED) {
        CheckForScreenCollisions();
    }
    // Consider: disable PARTICLE_COLLISION_ENABLED for fluid particles
    // (SPH pressure forces handle separation)

    // ── STEP 6: Integration ──
    if (INTEGRATION_ENABLED) {
        for (auto particle: particles) {
            particle->RK4Integrate(dt);
        }
    }
}
```

### Critical Dependencies

**Why this order matters:**
1. **Octree before density:** Neighbor search needs updated spatial structure
2. **Density before pressure:** Pressure equation P = k(ρ - ρ₀) needs ρ
3. **Pressure before forces:** Force equation needs both Pᵢ and Pⱼ
4. **Forces before integration:** Integration consumes accumulated forces
5. **Boundary after forces:** Prevents particles from leaving domain before integration

**Performance Consideration:**
- Neighbor search happens twice (density + forces)
- **Optimization:** Cache neighbor lists in step 2, reuse in step 3
  - Trade memory for speed
  - Implement later if profiling shows bottleneck

---

## 3D Generalization

### Design Patterns for 2D→3D Extension

The architecture must support future 3D without major rewrites. Key abstraction points:

#### 1. Template-Based Solver (Recommended Approach)

**Current 2D Implementation:**
```cpp
class SPHSolver {
    void ComputeDensityPressure(std::vector<Particle2D*>& particles, Octree2D* octree);
    void ComputeForces(std::vector<Particle2D*>& particles, Octree2D* octree);
};
```

**3D-Ready Template:**
```cpp
template<typename ParticleType, typename SpatialStructure>
class SPHSolver {
    void ComputeDensityPressure(std::vector<ParticleType*>& particles, SpatialStructure* spatial);
    void ComputeForces(std::vector<ParticleType*>& particles, SpatialStructure* spatial);
};

// Instantiations:
using SPHSolver2D = SPHSolver<Particle2D, Octree2D>;
using SPHSolver3D = SPHSolver<Particle3D, Octree3D>;  // Future
```

**Pros:**
- Single codebase for 2D/3D logic
- Forces abstraction of particle/spatial interfaces
- Compile-time optimization (no virtual function overhead)

**Cons:**
- Header-heavy (template implementations in .hpp)
- Compiler errors can be cryptic
- May be overkill for first milestone

**Recommendation for Phase 1:** Start with concrete `SPHSolver2D`, refactor to template in Phase 2 (3D milestone).

#### 2. Kernel Functions Already Dimension-Agnostic

**SPHKernel.hpp is already genericized:**
```cpp
// Works for 2D and 3D
static float Poly6(const Vec2& r);  // ← Takes vector, returns scalar
static float Poly6(const Vec3& r);  // ← Overload for Vec3

static Vec2 SpikyGradient(const Vec2& r);  // ← Returns same dimension
static Vec3 SpikyGradient(const Vec3& r);  // ← Overload for Vec3
```

**Current status:** Only Vec2 overloads exist. Add Vec3 overloads when 3D arrives.

**Kernel math is identical in 2D/3D:**
- Poly6: W(r, h) = (315/64πh⁹)(h² - r²)³
- Spiky gradient magnitude: same formula
- Direction: normalize(rᵢⱼ) works in any dimension

**Only change:** Normalization constants differ (2D uses different π scaling).

#### 3. Particle Interface Compatibility

**Existing Particle2D and Particle3D share identical structure:**

| Feature | Particle2D | Particle3D | Compatibility |
|---------|-----------|-----------|---------------|
| Position | Vec2 | Vec3 | ✓ Same member name |
| Velocity | Vec2 | Vec3 | ✓ Same member name |
| Mass, invMass | float | float | ✓ Identical |
| Radius | float | float | ✓ Identical |
| SPH props | density, pressure | **Missing** | ✗ Need to add |
| Integration | RK4Integrate(dt) | **Missing RK4** | ✗ Need to add |

**Action Items for 3D-readiness:**
1. Add SPH properties to Particle3D.hpp:
   ```cpp
   float density, pressure;
   Vec3 pressureForce, viscosityForce;
   ```
2. Add RK4Integrate() to Particle3D (currently only has Euler/Verlet)
3. Ensure Force::GenerateWeightForce() works with Particle3D

**These can wait until 3D milestone.**

#### 4. Spatial Structure Interface

**Current:** Octree2D has suitable interface for abstraction
```cpp
void findNeighbors(std::vector<Particle2D*>& neighbors, Particle2D* particle, double searchRadius);
```

**3D Equivalent:**
```cpp
// Hypothetical Octree3D (8 children instead of 4)
void findNeighbors(std::vector<Particle3D*>& neighbors, Particle3D* particle, double searchRadius);
```

**Shared Interface Pattern:**
```cpp
// Abstract spatial query interface
template<typename ParticleType>
class SpatialIndex {
public:
    virtual void rebuild(std::vector<ParticleType*>& particles) = 0;
    virtual void findNeighbors(
        std::vector<ParticleType*>& neighbors,
        ParticleType* query,
        double radius
    ) = 0;
};

// Concrete implementations
class Octree2D : public SpatialIndex<Particle2D> { ... };
class Octree3D : public SpatialIndex<Particle3D> { ... };
```

**Recommendation:** Don't abstract yet. Current Octree2D interface is clean enough to duplicate for Octree3D when needed.

#### 5. System/Scene Hierarchy

**Current structure works for both dimensions:**
```
System (base class)
  ├─ WaterDam2D : System    ← Uses Particle2D, SPHSolver2D
  └─ WaterDam3D : System    ← Future: Uses Particle3D, SPHSolver3D
```

**System.hpp already dimension-agnostic in places:**
- `std::vector<Particle2D*> particles` ← Would become template parameter or separate System2D/System3D
- Octree2D* octree ← Same issue

**3D-Ready Refactor (later):**
```cpp
template<int Dim>
class System {
    using ParticleType = std::conditional_t<Dim == 2, Particle2D, Particle3D>;
    using VecType = std::conditional_t<Dim == 2, Vec2, Vec3>;
    using OctreeType = std::conditional_t<Dim == 2, Octree2D, Octree3D>;

    std::vector<ParticleType*> particles;
    OctreeType* octree;
    SPHSolver<ParticleType, OctreeType>* sphSolver;
};
```

**But for Phase 1:** Concrete classes are fine. Refactor when 3D becomes priority.

---

## Summary of Abstractions to Implement Now

### Must Have (Phase 1 - WCSPH 2D)
1. **SPHSolver class** (new file: SPHSolver2D.hpp/cpp)
   - Encapsulates density/pressure/force computation
   - Owned by System, called from Advect()
2. **SPHKernel initialization** (already exists, just call Initialize())
3. **Particle2D methods** (already stubbed, implement or delegate to SPHSolver)
4. **Constants consolidation** (add SPH_SMOOTHING_LENGTH to Constants.hpp)

### Nice to Have (Phase 1)
1. **Neighbor caching** (std::unordered_map<Particle2D*, std::vector<Particle2D*>>)
   - Reuse between density and force computations
   - Only if profiling shows repeated neighbor search is bottleneck

### Defer to Phase 2 (3D Extension)
1. Template-based SPHSolver<ParticleType, SpatialType>
2. Octree3D implementation
3. Particle3D SPH property additions
4. Vec3 overloads for SPHKernel
5. System2D/System3D split or template

---

## Integration Checklist

### Quality Gate Validation

- [x] **Integration points with existing code clear**
  - SPHSolver integrates into System::Advect()
  - Uses existing Particle2D for data storage
  - Uses existing Octree2D for neighbor search
  - Uses existing SPHKernel for kernel math
  - Coexists with existing Force generators

- [x] **Update order explicit**
  - 6-step pipeline documented:
    1. Spatial structure update
    2. Density & pressure computation
    3. SPH force computation
    4. External forces (gravity, etc.)
    5. Boundary conditions
    6. Integration
  - Dependencies and rationale explained

- [x] **3D generalization path noted**
  - Template-based solver approach outlined
  - Kernel functions already dimension-agnostic
  - Particle interface compatibility analyzed
  - Spatial structure interface pattern suggested
  - Concrete 2D implementation recommended for Phase 1
  - Template refactor deferred to Phase 2

---

## Suggested Build Order (Phase Dependencies)

```
Phase 1: Core WCSPH Components
├─ 1.1: SPHSolver class skeleton
├─ 1.2: Density computation (Poly6 kernel)
├─ 1.3: Pressure computation (Tait equation)
├─ 1.4: Pressure forces (Spiky kernel)
└─ 1.5: Viscosity forces (Viscosity kernel)

Phase 2: System Integration
├─ 2.1: Add SPHSolver to System class
├─ 2.2: Integrate into Advect() pipeline
├─ 2.3: Toggle FLUID_DYNAMICS_ENABLED
└─ 2.4: Test with WaterDam scene

Phase 3: Optimization
├─ 3.1: Profile neighbor search overhead
├─ 3.2: Implement neighbor caching (if needed)
└─ 3.3: Octree tuning (maxDepth, maxParticles)

Phase 4: Boundary & Rendering
├─ 4.1: Boundary particle handling
├─ 4.2: Screen collision for fluids
└─ 4.3: Metaball rendering (separate from solver)

Phase 5: Polish & Parameters
├─ 5.1: Runtime parameter tuning UI
├─ 5.2: Particle spawning/removal
└─ 5.3: Performance validation (60fps @ 1000+ particles)
```

**Critical path:** Phase 1 → Phase 2 must complete before Phase 4 (boundary/rendering).
Phase 3 can happen in parallel with Phase 4.

---

*Document Status: Complete*
*Architecture validated against:*
- Existing codebase structure (Application → System → Physics → Graphics)
- SPH algorithm requirements (neighbor search, density, pressure, forces)
- Performance constraints (60fps, thousands of particles)
- 3D extension goals (templates, dimension-agnostic kernels)
