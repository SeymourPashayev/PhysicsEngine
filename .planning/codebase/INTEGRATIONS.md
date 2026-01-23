# External Integrations

**Analysis Date:** 2026-01-23

## APIs & External Services

**Not Applicable**

This physics engine does not integrate with external APIs or web services. All simulation is self-contained and runs locally.

## Data Storage

**Databases:**
- None - In-memory only
- Particle and spring data stored in `std::vector` containers within `System` class
- Located: `src/System.hpp`

**File Storage:**
- Local filesystem only
- No persistent storage of simulation state
- No save/load functionality implemented

**Caching:**
- None - All calculations performed in-memory per frame

## Authentication & Identity

**Auth Provider:**
- Not applicable - No external authentication
- Standalone desktop application with no user accounts or authentication

## Monitoring & Observability

**Error Tracking:**
- Console output only (std::cout)
- No external error tracking service
- Debug logging in `src/Inputs/Mouse.hpp` and `src/Physics/Octree2D.cpp` via iostream

**Logs:**
- Standard output (console)
- Logging via `std::cout` for:
  - Mouse input events
  - Octree operations (debug)
  - Particle creation/collision events
  - Located: Various source files using `<iostream>`

## CI/CD & Deployment

**Hosting:**
- None - Standalone desktop application
- User compiles locally from source with provided Makefile

**CI Pipeline:**
- None configured
- Manual build via `make build`

**Distribution:**
- Source code distribution
- Users compile with their own compiler/SDL2 libraries

## Environment Configuration

**Required env vars:**
- None - Application requires no environment variables

**Secrets location:**
- Not applicable - No authentication or secrets management

## System Integration

**Graphics Display:**
- SDL2 window system integration
- Window size: Configurable via Graphics class
- Renderer: SDL2_Renderer for cross-platform rendering
- Mouse input: SDL2 event system
- Located: `src/Graphics/Graphics.hpp`, `src/Inputs/Mouse.hpp`

**Operating System:**
- Cross-platform SDL2 abstraction layer
- Works on: macOS, Linux, Windows (where SDL2 libraries available)
- Tested: macOS (Darwin 24.5.0, x86_64)

## Webhooks & Callbacks

**Incoming:**
- None - Standalone simulation engine

**Outgoing:**
- None - No outbound communications

## Physics Simulation Output

**Real-time Rendering:**
- Particles rendered to SDL2 window each frame
- Graphics pipeline: `Graphics::ClearScreen()` → Draw particles → `Graphics::RenderFrame()`
- No export of simulation state to external formats

## Scene Management

**Scene Switching:**
- Application supports multiple scene types
- Scenes inherit from `System` base class
- Implemented scenes:
  - `SpringForceLattice`: Spring-connected particle grid
  - `WaterDam`: Water fluid simulation test case
- Located: `src/Scenes/SpringForceLattice.hpp`, `src/Scenes/WaterDam.hpp`
- Switching logic in `Application` class: `src/Application.hpp`

---

*Integration audit: 2026-01-23*
