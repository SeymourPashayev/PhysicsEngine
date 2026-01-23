# Technology Stack

**Analysis Date:** 2026-01-23

## Languages

**Primary:**
- C++17 - Core physics engine and simulation logic, all source in `/Users/seymour/projects/PhysicsEngine/src/`

## Runtime

**Environment:**
- Native macOS/Linux/Windows (cross-platform C++ executable)
- Compiled to standalone binary (`engine` executable)

**Compiler:**
- g++ (GCC/Clang compatible)
- C++17 standard (-std=c++17 flag required)

**Package Manager:**
- Native dependency management via Makefile
- No third-party package manager (npm, pip, cargo, etc.)

## Build System

**Build Tool:**
- GNU Make
- Config: `/Users/seymour/projects/PhysicsEngine/Makefile`

**Build Command:**
```bash
make build
# Compiles with: g++ -std=c++17 -Wall ./src/*.cpp ./src/Physics/*.cpp ./src/Inputs/*.cpp ./src/Scenes/*.cpp ./src/Graphics/*.cpp -lm -lSDL2 -lSDL2_gfx -o engine
```

**Runtime Commands:**
```bash
make run    # Execute the compiled engine
make clean  # Remove engine executable
```

## Core Dependencies

**Graphics & Rendering:**
- SDL2 (Simple DirectMedia Layer 2) - Window management, rendering context
  - Linked with: -lSDL2
  - Headers: `<SDL2/SDL.h>`, `<SDL2/SDL_stdinc.h>`, `<SDL2/SDL_timer.h>`
  - Used in: `src/Graphics/Graphics.hpp`

- SDL2_gfx (SDL2 graphics primitives extension)
  - Linked with: -lSDL2_gfx
  - Headers: `<SDL2/SDL2_gfxPrimitives.h>`
  - Provides: Circle, line, rectangle, polygon drawing functions
  - Used in: `src/Graphics/Graphics.hpp`

- SDL2_image
  - Referenced in documentation but not actively linked in current Makefile
  - May be planned for future texture support

**Standard Library:**
- `<vector>` - Dynamic particle and spring storage
- `<cmath>` - Math operations (sin, cos, sqrt, etc.)
- `<iostream>` - Console output and debugging
- `<algorithm>` - STL algorithms (used in force calculations)
- `<atomic>` - Atomic operations for thread-safe spatial partitioning
- `<functional>` - Function objects for callbacks
- `<memory>` - Smart pointers (shared_ptr)
- `<mutex>` - Thread synchronization for Octree2D
- `<math.h>` - C math library functions

**Math Library:**
- Linked with: -lm (math library)
- Provides: Mathematical functions for physics calculations

## Core Physics Libraries (Internal)

**Custom Math:**
- `Vec2` class: 2D vector with operators, normalization, dot product, cross product
- `Vec3` class: 3D vector mathematics
- Location: `src/Physics/Vec2.hpp`, `src/Physics/Vec3.hpp`

**Particle System:**
- `Particle2D`: Point mass representation (position, velocity, acceleration, mass, radius, color)
- `Particle3D`: 3D particle variant
- Location: `src/Physics/Particle2D.hpp`, `src/Physics/Particle3D.hpp`

**Force Calculations:**
- `Force` class: Comprehensive force system
  - Gravitational forces (planetary and inter-particle)
  - Friction and drag forces
  - Spring forces
  - Located: `src/Physics/Force.hpp`

**Springs:**
- `Spring` class: Elastic constraints between particles
- Location: `src/Physics/Spring.hpp`

**Spatial Acceleration:**
- `Octree2D`: Spatial partitioning for O(log n) neighbor queries
  - Thread-safe with mutex/atomic operations
  - Used for collision detection and force calculations
  - Located: `src/Physics/Octree2D.hpp`

**SPH (Smoothed Particle Hydrodynamics):**
- `SPHKernel` class: Kernel functions for fluid simulation
  - Poly6 kernel (density calculation)
  - Spiky kernel gradient (pressure forces)
  - Viscosity kernel laplacian
  - Located: `src/Physics/SPHKernel.hpp`

## Configuration

**Physics Constants:**
- Centralized in `src/Physics/Constants.hpp`
- FPS: 60
- Frame time: ~16.67ms
- PIXELS_PER_METER: 4000 (for screen-to-world scaling)
- GRAVITY: 9.81 m/s²
- GRAVITATIONAL_CONSTANT: 6.675
- REST_DENSITY: 1000.0 (fluid)
- STIFFNESS: 100.0 (spring parameter)
- VISCOSITY: 0.5 (fluid property)

**No Environment Variables Required:**
- Static compilation with hardcoded constants
- All parameters in `Constants.hpp`

## Input System

**Mouse Input:**
- SDL2 mouse event handling
- Left-click particle creation at cursor position
- Located: `src/Inputs/Mouse.hpp`

## Graphics System

**Rendering:**
- SDL2 software/hardware renderer
- Drawing primitives: lines, circles, rectangles, polygons
- Static Graphics class: `src/Graphics/Graphics.hpp`
- Supports texture rendering (planned feature)

## Integration Methods

**Supported Physics Solvers:**
- Runge-Kutta 4th Order (RK4) - Default, highest accuracy
- Euler - Simple explicit method
- Verlet - Position-based alternative

## Platform Requirements

**Development:**
- C++17 compatible compiler (g++, clang)
- SDL2 development libraries with headers
- SDL2_gfx development libraries
- GNU Make
- Tested on: macOS (x86_64), Darwin 24.5.0

**Production:**
- SDL2 runtime libraries installed
- 64-bit processor (x86_64)
- Display hardware for SDL window rendering

## Special Features in Development

**SPH/FLIP Methods:**
- Active development of particle-in-cell fluid simulation
- Adaptive grid resolution planned
- See `TODO` file for Gaussian kernel implementation tasks

---

*Stack analysis: 2026-01-23*
