# Requirements: SPH Fluid Simulation Engine

**Defined:** 2026-01-23
**Core Value:** Fluid that looks and behaves convincingly at interactive frame rates

## v1 Requirements

Requirements for initial release. Each maps to roadmap phases.

### Core SPH

- [x] **SPH-01**: Particles have density calculated from neighbors using Poly6 kernel
- [x] **SPH-02**: Particles have pressure calculated from density using Tait equation of state
- [x] **SPH-03**: Pressure forces push particles apart using Spiky kernel gradient
- [x] **SPH-04**: Viscosity forces smooth velocity differences between neighbors
- [x] **SPH-05**: Neighbor search finds all particles within smoothing radius efficiently

### Stability

- [ ] **STAB-01**: Density diffusion (delta-SPH) eliminates pressure oscillations
- [ ] **STAB-02**: CFL condition enforces adaptive timestep for stability
- [ ] **STAB-03**: Boundary particles prevent fluid from escaping container
- [x] **STAB-04**: Symplectic Euler integration preserves energy over time

### Rendering

- [ ] **REND-01**: Metaball rendering shows fluid as smooth continuous surface
- [ ] **REND-02**: Color field visualizes pressure or velocity magnitude
- [ ] **REND-03**: Particle rendering mode shows individual particles (debug/toggle)
- [ ] **REND-04**: Rendering mode switchable at runtime

### Interactivity

- [ ] **INT-01**: Click to spawn new fluid particles at cursor position
- [ ] **INT-02**: Runtime slider/key controls for viscosity parameter
- [ ] **INT-03**: Runtime slider/key controls for stiffness parameter
- [ ] **INT-04**: Keyboard toggle between rendering modes

### Performance

- [ ] **PERF-01**: Simulation runs at 60fps with 1,000 particles
- [ ] **PERF-02**: Simulation runs at 60fps with 5,000 particles
- [ ] **PERF-03**: Neighbor search completes in under 2ms per frame

### Demo Scene

- [ ] **DEMO-01**: Dam break scene with fluid column that collapses realistically
- [ ] **DEMO-02**: Rectangular container with solid walls on bottom and sides
- [ ] **DEMO-03**: Initial fluid configuration as tall column on one side

### Architecture

- [x] **ARCH-01**: SPH solver separated from particle data structure
- [x] **ARCH-02**: Kernel functions work with any vector type (2D/3D ready)
- [x] **ARCH-03**: Neighbor search interface abstracts spatial data structure

## v2 Requirements

Deferred to future release. Tracked but not in current roadmap.

### Enhanced Interactivity

- **INT-05**: Click to place solid obstacles in fluid
- **INT-06**: Drag to create streams of particles
- **INT-07**: Preset scenes (dam break, double dam, pouring)

### Visual Enhancements

- **REND-05**: Surface tension for droplet beading
- **REND-06**: Splash/spray particles at high-velocity surfaces
- **REND-07**: Multiple fluid colors/phases

### 3D Extension

- **3D-01**: 3D particle simulation with SPH
- **3D-02**: 3D metaball rendering (marching cubes)
- **3D-03**: Camera controls for 3D view

## Out of Scope

Explicitly excluded. Documented to prevent scope creep.

| Feature | Reason |
|---------|--------|
| GPU/CUDA acceleration | Keeping CPU-only to reduce complexity; can add later |
| Physical accuracy | Visual plausibility sufficient for showcase |
| Incompressible SPH (PCISPH, IISPH) | WCSPH simpler and adequate for visual demo |
| PIC/FLIP methods | WCSPH chosen as simpler path |
| Arbitrary obstacle geometry | Rectangular container sufficient for dam break |
| Multi-threading | Optimization can come after single-threaded works |

## Traceability

Which phases cover which requirements. Updated during roadmap creation.

| Requirement | Phase | Status |
|-------------|-------|--------|
| SPH-01 | Phase 1 | Complete |
| SPH-02 | Phase 1 | Complete |
| SPH-03 | Phase 1 | Complete |
| SPH-04 | Phase 1 | Complete |
| SPH-05 | Phase 1 | Complete |
| STAB-01 | Phase 3 | Pending |
| STAB-02 | Phase 3 | Pending |
| STAB-03 | Phase 2 | Pending |
| STAB-04 | Phase 1 | Complete |
| REND-01 | Phase 4 | Pending |
| REND-02 | Phase 4 | Pending |
| REND-03 | Phase 4 | Pending |
| REND-04 | Phase 4 | Pending |
| INT-01 | Phase 4 | Pending |
| INT-02 | Phase 4 | Pending |
| INT-03 | Phase 4 | Pending |
| INT-04 | Phase 4 | Pending |
| PERF-01 | Phase 3 | Pending |
| PERF-02 | Phase 3 | Pending |
| PERF-03 | Phase 3 | Pending |
| DEMO-01 | Phase 2 | Pending |
| DEMO-02 | Phase 2 | Pending |
| DEMO-03 | Phase 2 | Pending |
| ARCH-01 | Phase 1 | Complete |
| ARCH-02 | Phase 1 | Complete |
| ARCH-03 | Phase 1 | Complete |

**Coverage:**
- v1 requirements: 26 total
- Mapped to phases: 26
- Unmapped: 0 ✓

---
*Requirements defined: 2026-01-23*
*Last updated: 2026-01-23 after roadmap creation*
