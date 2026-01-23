# SPH Fluid Simulation Engine

## What This Is

A 2D SPH (Smoothed Particle Hydrodynamics) fluid simulation engine built on an existing C++ particle physics framework. The goal is a visually impressive dam break simulation with metaball rendering, handling thousands of particles at 60fps. Architecture is designed for future 3D extension.

## Core Value

Fluid that looks and behaves convincingly at interactive frame rates — visual plausibility over physical exactness.

## Requirements

### Validated

- ✓ Particle system with position, velocity, acceleration, mass — existing
- ✓ Vector math library (Vec2, Vec3) — existing
- ✓ Force calculations (gravity, friction, drag, spring) — existing
- ✓ Spatial partitioning via Octree2D — existing
- ✓ Multiple integration methods (RK4, Euler, Verlet) — existing
- ✓ Scene system with switchable simulations — existing
- ✓ SDL2-based rendering (circles, lines, primitives) — existing
- ✓ Mouse input for particle creation — existing
- ✓ Game loop architecture (Input → Update → Render) — existing

### Active

- [ ] WCSPH pressure solver with density/pressure calculations
- [ ] SPH kernel functions (Poly6 for density, Spiky for pressure, Viscosity for diffusion)
- [ ] Neighbor search optimized for SPH (spatial hashing or octree adaptation)
- [ ] Viscosity forces for fluid cohesion
- [ ] Surface tension approximation
- [ ] Boundary handling (solid walls, container)
- [ ] Dam break scene with fluid column collapse
- [ ] Metaball/marching squares rendering for fluid surface
- [ ] Interactive particle spawning (click to add fluid)
- [ ] Interactive obstacle placement
- [ ] Runtime parameter tuning (viscosity, stiffness, surface tension)
- [ ] Performance: 1,000-5,000 particles at 60fps
- [ ] Architecture patterns that generalize to 3D

### Out of Scope

- GPU/CUDA acceleration — keeping CPU-only for this milestone
- 3D rendering — 2D visualization only, but code structure should allow 3D later
- Physical accuracy for research — visual plausibility is sufficient
- Incompressible SPH variants (PCISPH, IISPH) — WCSPH is simpler and adequate for visuals
- PIC/FLIP methods — mentioned in old TODO but WCSPH chosen instead

## Context

This builds on an existing physics engine with:
- Particle2D/3D classes with SPH properties (density, pressure) already stubbed
- SPHKernel.hpp/cpp files exist but are scaffolding only
- Octree2D for spatial queries (may need adaptation for SPH smoothing radius)
- WaterDam scene exists as starting point
- Constants.hpp has REST_DENSITY, STIFFNESS, VISCOSITY defined

The codebase uses a layered architecture:
- Application layer (game loop)
- System base class (physics state, toggles)
- Scene layer (concrete simulations)
- Physics layer (particles, forces, spatial structures)
- Graphics layer (SDL2 rendering)

## Constraints

- **Dependencies**: SDL2, SDL2_gfx only — no additional libraries
- **Platform**: macOS primary, should remain cross-platform (g++/clang)
- **Language**: C++17
- **Performance**: Must maintain 60fps with thousands of particles
- **Build**: Existing Makefile-based build system

## Key Decisions

| Decision | Rationale | Outcome |
|----------|-----------|---------|
| WCSPH over incompressible variants | Simpler implementation, adequate for visual showcase | — Pending |
| Metaball rendering | Fluid-like appearance vs particle circles | — Pending |
| CPU-only | Keeps complexity down, 3D GPU can come later | — Pending |
| 2D first with 3D-ready architecture | Foundation for scaling without premature complexity | — Pending |

---
*Last updated: 2026-01-23 after initialization*
