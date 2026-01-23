# Codebase Structure

**Analysis Date:** 2026-01-23

## Directory Layout

```
PhysicsEngine/
├── src/                          # All source code
│   ├── Main.cpp                  # Entry point with game loop
│   ├── Application.hpp           # Application class declaration
│   ├── Application.cpp           # Application class implementation
│   ├── System.hpp                # Base simulation system declaration
│   ├── System.cpp                # Base simulation system implementation
│   ├── Physics/                  # Core physics engine
│   │   ├── Constants.hpp         # Global physics constants
│   │   ├── Vec2.hpp              # 2D vector math
│   │   ├── Vec2.cpp              # 2D vector implementation
│   │   ├── Vec3.hpp              # 3D vector math
│   │   ├── Vec3.cpp              # 3D vector implementation
│   │   ├── Particle2D.hpp        # 2D particle definition
│   │   ├── Particle2D.cpp        # 2D particle implementation
│   │   ├── Particle3D.hpp        # 3D particle definition (not currently used)
│   │   ├── Particle3D.cpp        # 3D particle implementation (not currently used)
│   │   ├── Force.hpp             # Force calculation functions
│   │   ├── Force.cpp             # Force implementation
│   │   ├── Spring.hpp            # Spring constraint definition
│   │   ├── Spring.cpp            # Spring implementation
│   │   ├── Octree2D.hpp          # Spatial partitioning structure
│   │   ├── Octree2D.cpp          # Octree implementation
│   │   ├── SPHKernel.hpp         # SPH kernel functions (in development)
│   │   └── SPHKernel.cpp         # SPH kernel implementation (in development)
│   ├── Graphics/                 # Rendering layer
│   │   ├── Graphics.hpp          # Graphics utility class
│   │   └── Graphics.cpp          # Graphics SDL2 wrapper
│   ├── Inputs/                   # Input handling
│   │   ├── Mouse.hpp             # Mouse input tracking
│   │   └── Mouse.cpp             # Mouse implementation
│   └── Scenes/                   # Concrete simulation scenarios
│       ├── WaterDam.hpp          # Water dam simulation definition
│       ├── WaterDam.cpp          # Water dam initialization
│       ├── SpringForceLattice.hpp # Spring lattice simulation definition
│       └── SpringForceLattice.cpp # Spring lattice initialization
├── assets/                       # Graphics assets (textures, etc.)
├── .planning/                    # GSD planning documents
├── Makefile                      # Build configuration
├── CLAUDE.md                     # Claude AI project guidance
├── Constants.hpp                 # (appears to be legacy)
└── engine                        # Compiled executable
```

## Directory Purposes

**src/:**
- Purpose: All C++ source and header files
- Contains: Application logic, physics engine, rendering, input, scene definitions
- Key files: `Main.cpp` (entry), `Application.cpp` (loop), `System.cpp` (simulation)

**src/Physics/:**
- Purpose: Physics engine core implementations
- Contains: Vector math, particles, forces, collision, spatial structures, integration methods
- Key files: `Particle2D.hpp/cpp`, `Force.hpp/cpp`, `Octree2D.hpp/cpp`
- Note: Contains both completed (Vec2, Force, Particle2D) and in-development (SPHKernel) code

**src/Graphics/:**
- Purpose: SDL2 rendering abstraction
- Contains: Window management, primitive drawing (circles, lines, rects)
- Key files: `Graphics.hpp/cpp`

**src/Inputs/:**
- Purpose: Input device management
- Contains: Mouse state, position tracking, cursor size
- Key files: `Mouse.hpp/cpp`

**src/Scenes/:**
- Purpose: Concrete simulation scenarios
- Contains: Scene initialization, particle/spring setup
- Key files: `WaterDam.hpp/cpp`, `SpringForceLattice.hpp/cpp`

**assets/:**
- Purpose: Static graphics resources
- Contains: Textures, images (referenced by SDL2_image)
- Generated: No
- Committed: Yes

## Key File Locations

**Entry Points:**
- `src/Main.cpp`: Program entry; creates Application, runs loop
- `src/Application.cpp`: Game loop implementation; calls Input → Update → Render
- `src/System.cpp`: Simulation loop; particle updates, force calculations, collision detection

**Configuration:**
- `src/Physics/Constants.hpp`: All physics constants (gravity, FPS, pixels-per-meter, SPH parameters)
- `Makefile`: Build configuration with g++, C++17, SDL2 flags
- `CLAUDE.md`: Developer guidance

**Core Logic:**
- `src/System.cpp`: Main simulation update via Advect method
- `src/Physics/Particle2D.cpp`: Integration methods (RK4, Euler, Verlet), collision resolution
- `src/Physics/Force.cpp`: Force calculations (gravity, friction, drag, spring, SPH)
- `src/Physics/Octree2D.cpp`: Spatial partitioning for neighbor queries

**Rendering:**
- `src/Graphics/Graphics.cpp`: SDL2 window management and drawing primitives
- `src/System.cpp`: Draw method iterates particles and calls Graphics::DrawFillCircle

**Input Handling:**
- `src/Application.cpp`: SDL_PollEvent loop capturing keyboard, mouse, window events
- `src/Inputs/Mouse.cpp`: Mouse position and click state tracking

**Testing:**
- Not detected; no test files in repository

## Naming Conventions

**Files:**
- Headers: `ClassName.hpp` (or `.h` for legacy files like `Vec2_hpp`)
- Implementations: `ClassName.cpp`
- Capitalized PascalCase for class files
- Example: `Particle2D.hpp`, `Force.hpp`, `Graphics.hpp`

**Directories:**
- PascalCase: `Physics`, `Graphics`, `Inputs`, `Scenes`
- Lowercase: `src`, `assets`

**Classes/Structs:**
- PascalCase: `Application`, `System`, `Particle2D`, `Vec2`, `Octree2D`, `Mouse`, `Graphics`
- All physics entities use PascalCase

**Member Variables:**
- camelCase: `particleCount`, `pushForce`, `screenColour`, `scrollRadius`
- Constants: UPPERCASE_WITH_UNDERSCORES: `GRAVITY`, `INTEGRATION_ENABLED`, `MAX_SEARCH_RADIUS`

**Functions:**
- PascalCase: `Setup()`, `Update()`, `Render()`, `Advect()`, `RK4Integrate()`, `CheckForScreenCollisions()`
- Verb-first naming: `GenerateWeightForce()`, `ComputeDensity()`, `FindNeighbors()`

**Types:**
- 2D/3D particle versions: `Particle2D`, `Particle3D`, `Vec2`, `Vec3`
- Descriptive class names: `Octree2D`, `SpringForceLattice`, `WaterDam`

## Where to Add New Code

**New Physics Feature:**
- Core implementation: `src/Physics/NewFeature.hpp` and `NewFeature.cpp`
- If it's a force type: Add static method to `src/Physics/Force.hpp/cpp`
- If it's a data structure: New header/cpp pair in `src/Physics/`
- Register in: `src/System.hpp` (add toggle flag and call in Advect)

**New Simulation Scene:**
- Definition: `src/Scenes/NewScene.hpp` inheriting from System
- Implementation: `src/Scenes/NewScene.cpp` with constructor particle setup
- Activate in: `src/Application.cpp` line 31 (instantiate in Setup)

**New Input Type:**
- Add to: `src/Inputs/` with new header/cpp pair
- Wire in: `src/Application.cpp` Input method
- Pass reference to: Scenes via System constructor

**Bug Fixes / Modifications:**
- Physics constants: Edit `src/Physics/Constants.hpp`
- Integration methods: Edit `src/Physics/Particle2D.cpp` (EulerIntegrate, VerletIntegrate, RK4Integrate methods)
- Force calculations: Edit `src/Physics/Force.cpp`
- Collision detection: Edit `src/Physics/Particle2D.cpp` (CheckCollision, ResolveCollision methods) or `src/System.cpp` (CheckForScreenCollisions, CheckForParticleCollisions)
- Rendering: Edit `src/Graphics/Graphics.cpp` or `src/System.cpp` Draw method

**Utilities / Helpers:**
- Math utilities: Add to `src/Physics/Vec2.cpp` or `Vec3.cpp`
- Debug output: Implement in relevant class (e.g., Mouse::printCurrentLocation in Inputs/Mouse.cpp)

## Special Directories

**src/Physics/:**
- Purpose: Contains physics engine; most time-critical code
- Generated: No
- Committed: Yes
- Note: Includes in-development SPHKernel for future smoothed particle hydrodynamics

**assets/:**
- Purpose: Static resources (images, textures)
- Generated: No
- Committed: Yes
- Note: Used by Graphics layer via SDL2_image

**.planning/codebase/:**
- Purpose: GSD codebase documentation
- Generated: Yes (by GSD mappers)
- Committed: Yes
- Note: Contains ARCHITECTURE.md, STRUCTURE.md, and other analysis docs

---

*Structure analysis: 2026-01-23*
