# Architecture

**Analysis Date:** 2026-01-23

## Pattern Overview

**Overall:** Layered game loop architecture with scene-based simulation systems

**Key Characteristics:**
- Standard game loop (Input → Update → Render) managed by `Application`
- Multiple simulation scenes inheriting from `System` base class
- Physics engine decoupled from rendering layer
- Spatial partitioning via Octree2D for efficient neighbor queries
- Force-based particle dynamics with multiple integration methods

## Layers

**Application Layer:**
- Purpose: Main loop orchestration and scene management
- Location: `src/Application.cpp`, `src/Application.hpp`, `src/Main.cpp`
- Contains: Game loop control, input dispatch, scene switching
- Depends on: System, Graphics, Mouse
- Used by: Main entry point

**Simulation Layer (System):**
- Purpose: Core particle physics simulation and state management
- Location: `src/System.cpp`, `src/System.hpp`
- Contains: Particle collection, force calculations, collision detection, spatial indexing
- Depends on: Particle2D, Force, Octree2D, Spring, Mouse, Graphics
- Used by: Scenes (WaterDam, SpringForceLattice), Application

**Scene Layer:**
- Purpose: Concrete simulation implementations with specific particle configurations
- Location: `src/Scenes/WaterDam.cpp`, `src/Scenes/WaterDam.hpp`, `src/Scenes/SpringForceLattice.cpp`, `src/Scenes/SpringForceLattice.hpp`
- Contains: Particle initialization, scene-specific physics toggles
- Depends on: System (via inheritance)
- Used by: Application

**Physics Engine Layer:**
- Purpose: Core physics computations and data structures
- Location: `src/Physics/`
- Contains: Particle2D, Vec2, Vec3, Force, Spring, Octree2D, Constants
- Depends on: Only SDL2 types
- Used by: System, Scenes

**Graphics Layer:**
- Purpose: SDL2-based rendering abstraction
- Location: `src/Graphics/Graphics.cpp`, `src/Graphics/Graphics.hpp`
- Contains: Static rendering functions for primitives
- Depends on: SDL2, Vec2
- Used by: System (drawing particles), Application (frame rendering)

**Input Layer:**
- Purpose: Mouse state tracking and position management
- Location: `src/Inputs/Mouse.cpp`, `src/Inputs/Mouse.hpp`
- Contains: Mouse position, click states, cursor size
- Depends on: Vec2
- Used by: Application, System

## Data Flow

**Frame Update Cycle:**

1. **Input Phase** (Application::Input)
   - SDL_PollEvent captures keyboard, mouse, and window events
   - Mouse state updated via Mouse object
   - Input routed to scene toggle functions (gravity, integration, etc.)

2. **Update Phase** (Application::Update)
   - Frame timing calculated (deltaTime clamped to max 0.0016s)
   - Scene::Advect called with deltaTime and accumulated push force

3. **Advect Phase** (System::Advect)
   - Octree spatial structure rebuilt/updated
   - Smoothing lengths calculated for all particles (SPH preparation)
   - Screen collision detection and resolution
   - Particle-to-particle collision detection
   - Mouse click creates particles at cursor position
   - For each particle:
     - Arrow key push force applied (if keyboard enabled)
     - Gravity applied (if gravity enabled)
     - Friction applied (if friction enabled)
     - Attraction force via octree neighbor query (if attraction enabled)
     - Drag applied (if drag enabled)
     - Integration method (RK4/Euler/Verlet) applied
   - Spring forces applied between connected particles (if spring forces enabled)

4. **Render Phase** (Application::Render)
   - Graphics::ClearScreen fills with background
   - System::Draw renders all particles as filled circles
   - Graphics::RenderFrame presents buffer to SDL2

**State Management:**

- **Particle State:** Position, velocity, acceleration maintained per particle; forces accumulated then integrated
- **Scene State:** Particle collection, spring collection, octree spatial index
- **Simulation Toggles:** 9 boolean flags control enable/disable of physics subsystems
- **Input State:** Mouse holds position and click state; Application aggregates into push force

## Key Abstractions

**Particle2D:**
- Purpose: Point mass entity with position, velocity, mass, radius, collision properties
- Examples: `src/Physics/Particle2D.hpp`, `src/Physics/Particle2D.cpp`
- Pattern: Owns integration methods (Euler, Verlet, RK4), force accumulation, collision detection, SPH properties (density, pressure)

**Force:**
- Purpose: Static collection of force calculation functions
- Examples: `src/Physics/Force.hpp`, `src/Physics/Force.cpp`
- Pattern: All functions static; take particles as const references; return Vec2 force vectors

**Octree2D:**
- Purpose: Spatial partitioning for O(log n) neighbor finding
- Examples: `src/Physics/Octree2D.hpp`, `src/Physics/Octree2D.cpp`
- Pattern: Recursive quad-tree; max depth 6; leaf nodes store up to 16 particles; supports radius queries

**Vec2/Vec3:**
- Purpose: Vector mathematics library
- Examples: `src/Physics/Vec2.hpp`, `src/Physics/Vec3.hpp`
- Pattern: Value types with operator overloading; Normalize, Magnitude, Dot, Cross operations

**System (Base Class):**
- Purpose: Abstract simulation state and physics update loop
- Examples: `src/System.hpp`, `src/System.cpp`
- Pattern: Holds particle/spring collections, octree, toggle flags; virtual Advect method overridden by scenes

**Graphics (Static Utility):**
- Purpose: SDL2 rendering wrapper
- Examples: `src/Graphics/Graphics.hpp`, `src/Graphics/Graphics.cpp`
- Pattern: All methods static; manages global window/renderer; drawing primitives (circles, lines, rects)

## Entry Points

**Main:**
- Location: `src/Main.cpp`
- Triggers: Program execution
- Responsibilities: Creates Application, calls Setup, runs loop, calls Destroy

**Application::Setup:**
- Location: `src/Application.cpp`
- Triggers: Called once at startup
- Responsibilities: Initialize Graphics window, create Mouse object, instantiate scene (WaterDam by default)

**Application::Input:**
- Location: `src/Application.cpp`
- Triggers: Once per frame before update
- Responsibilities: Poll SDL events, update mouse state, toggle physics flags, accumulate keyboard force

**Application::Update:**
- Location: `src/Application.cpp`
- Triggers: Once per frame after input
- Responsibilities: Calculate deltaTime, call scene->Advect, manage frame rate via SDL_Delay

**Application::Render:**
- Location: `src/Application.cpp`
- Triggers: Once per frame after update
- Responsibilities: Call scene->Draw, call Graphics::RenderFrame to present

## Error Handling

**Strategy:** Minimal; relies on constructor success and valid pointers

**Patterns:**
- No exception handling; assumes all allocations succeed
- Null checks in loops before dereferencing particles (see `System::Draw` at line 130 in System.cpp)
- Screen boundary checks with clamping (see `System::CheckForScreenCollisions`)
- Hardcoded FIXME at line 140 in System.cpp indicates hardcoded window dimensions

## Cross-Cutting Concerns

**Logging:**
- Approach: cout to console during construction/destruction
- Examples: `std::cout << "System Initiated"` in System.cpp line 23, `std::cout << "Spring Force Lattice System Initiated"` in SpringForceLattice.cpp line 73

**Validation:**
- Approach: Particle collision checks via distance formula
- Particle radius bounds checked against screen dimensions in CheckForScreenCollisions
- No input validation on mouse cursor size bounds (has explicit min/max clamping in Mouse class)

**Physics Constants:**
- Approach: Centralized in `src/Physics/Constants.hpp`
- FPS = 60, MILLISECONDS_PER_FRAME = 16ms, PIXELS_PER_METER = 4000, GRAVITY = 9.81, GRAVITATIONAL_CONSTANT = 6.675, REST_DENSITY = 1000, STIFFNESS = 100, VISCOSITY = 0.5

---

*Architecture analysis: 2026-01-23*
