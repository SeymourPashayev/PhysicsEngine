# Roadmap: SPH Fluid Simulation Engine

## Overview

This roadmap transforms the existing particle physics engine into a WCSPH fluid simulator through four phases. Phase 1 establishes core SPH mechanics with kernels, neighbor search, and pressure forces. Phase 2 adds boundary handling for realistic dam break containment. Phase 3 ensures stability and performance through delta-SPH diffusion and optimization. Phase 4 delivers visual polish with metaball rendering and interactive parameter tuning. The result is a visually impressive fluid simulation handling thousands of particles at 60fps.

## Phases

**Phase Numbering:**
- Integer phases (1, 2, 3, 4): Planned milestone work
- Decimal phases (2.1, 2.2): Urgent insertions (marked with INSERTED)

Decimal phases appear between their surrounding integers in numeric order.

- [ ] **Phase 1: Core SPH Mechanics** - SPH kernels, neighbor search, pressure forces, and integration
- [ ] **Phase 2: Boundaries & Dam Break** - Boundary handling, containment, and dam break scene
- [ ] **Phase 3: Stability & Performance** - Delta-SPH diffusion, optimization, 60fps at 5000 particles
- [ ] **Phase 4: Visual Polish & Interactivity** - Metaball rendering, parameter tuning, color visualization

## Phase Details

### Phase 1: Core SPH Mechanics
**Goal**: Establish foundational SPH machinery that produces visible fluid motion
**Depends on**: Nothing (first phase)
**Requirements**: SPH-01, SPH-02, SPH-03, SPH-04, SPH-05, STAB-04, ARCH-01, ARCH-02, ARCH-03
**Success Criteria** (what must be TRUE):
  1. User can run simulation and see particles respond to SPH pressure forces
  2. Particles pushed apart when density exceeds rest density (incompressibility behavior visible)
  3. Particles fall under gravity and deform into blob shape when hitting ground
  4. Neighbor search finds all particles within smoothing radius in under 5ms per frame
  5. Symplectic Euler integration maintains stable motion without explosions
**Plans**: TBD

Plans:
- [ ] 01-01: TBD
- [ ] 01-02: TBD
- [ ] 01-03: TBD

### Phase 2: Boundaries & Dam Break
**Goal**: Contain fluid within solid walls and demonstrate realistic dam break scenario
**Depends on**: Phase 1
**Requirements**: STAB-03, DEMO-01, DEMO-02, DEMO-03
**Success Criteria** (what must be TRUE):
  1. Fluid stays contained within rectangular container boundaries (no leaks through walls)
  2. User can run dam break scene with tall fluid column on left side
  3. Dam collapse shows realistic wave propagation and splashing against walls
  4. Boundary particles prevent fluid penetration at corners and edges
**Plans**: TBD

Plans:
- [ ] 02-01: TBD
- [ ] 02-02: TBD

### Phase 3: Stability & Performance
**Goal**: Eliminate pressure oscillations and achieve 60fps with thousands of particles
**Depends on**: Phase 2
**Requirements**: STAB-01, STAB-02, PERF-01, PERF-02, PERF-03
**Success Criteria** (what must be TRUE):
  1. Fluid motion appears smooth without visible jitter or particle shaking
  2. Dam break simulation runs stable for 10+ seconds without explosions
  3. Simulation runs at 60fps with 1,000 particles on modern CPU
  4. Simulation runs at 60fps with 5,000 particles on modern CPU
  5. Neighbor search completes in under 2ms per frame
**Plans**: TBD

Plans:
- [ ] 03-01: TBD
- [ ] 03-02: TBD

### Phase 4: Visual Polish & Interactivity
**Goal**: Transform particle circles into fluid surface with interactive parameter control
**Depends on**: Phase 3
**Requirements**: REND-01, REND-02, REND-03, REND-04, INT-01, INT-02, INT-03, INT-04
**Success Criteria** (what must be TRUE):
  1. User sees fluid as smooth continuous surface (metaball rendering) not individual particles
  2. User can toggle between metaball and particle rendering modes at runtime
  3. Color field visualizes pressure or velocity magnitude for visual debugging
  4. User can click to spawn new fluid particles at cursor position
  5. User can adjust viscosity and stiffness parameters at runtime and see immediate effect
**Plans**: TBD

Plans:
- [ ] 04-01: TBD
- [ ] 04-02: TBD

## Progress

**Execution Order:**
Phases execute in numeric order: 1 → 2 → 3 → 4

| Phase | Plans Complete | Status | Completed |
|-------|----------------|--------|-----------|
| 1. Core SPH Mechanics | 0/TBD | Not started | - |
| 2. Boundaries & Dam Break | 0/TBD | Not started | - |
| 3. Stability & Performance | 0/TBD | Not started | - |
| 4. Visual Polish & Interactivity | 0/TBD | Not started | - |
