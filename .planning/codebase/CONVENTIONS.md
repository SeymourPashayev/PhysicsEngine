# Coding Conventions

**Analysis Date:** 2026-01-23

## Naming Patterns

**Files:**
- PascalCase for class/struct headers: `Particle2D.hpp`, `Application.hpp`
- Consistent `.cpp`/`.hpp` extension pairs for implementation and headers
- File names match primary class/struct name

**Functions:**
- PascalCase for function names: `EulerIntegrate()`, `GenerateDragForce()`, `CheckCollision()`
- Helper functions also PascalCase: `ClearForces()`, `Distance()`, `Normalize()`
- Getter functions follow PascalCase: `GetPosition()`, `GetLeftClick()`, `IsRunning()`
- Toggle functions use verb prefix: `ToggleIntegration()`, `ToggleGravity()`

**Variables:**
- camelCase for local and member variables: `position`, `velocity`, `acceleration`, `pushForce`, `dragCoefficient`, `restLength`
- UPPER_SNAKE_CASE for constants: `PIXELS_PER_METER`, `MILLISECONDS_PER_FRAME`, `GRAVITY`, `GRAVITATIONAL_CONSTANT`, `REST_DENSITY`
- Descriptive abbreviations: `dt` (delta time), `invMass` (inverse mass), `dpNorm`, `dpTan`

**Types:**
- PascalCase for struct names: `Vec2`, `Vec3`, `Particle2D`, `Spring`, `Force`, `Octree2D`
- No distinction in naming between class and struct (both PascalCase)

## Code Style

**Formatting:**
- g++ with C++17 standard (`-std=c++17`)
- No explicit formatter detected in codebase
- Manual formatting with consistent 4-space indentation observed
- Single space after control structures: `if (condition) {`

**Linting:**
- `-Wall` flag enabled during compilation to catch warnings
- No dedicated linter configuration found (no .eslintrc, clang-format, etc.)

## Import Organization

**Order (observed pattern in headers):**
1. Project-specific includes (local classes): `#include "Vec2.hpp"`, `#include "Particle2D.hpp"`
2. Relative path includes within project: `#include "./Graphics/Graphics.hpp"`, `#include "../Physics/Force.hpp"`
3. System includes: `#include <vector>`, `#include <iostream>`, `#include <cmath>`
4. Platform-specific includes: `#include <SDL2/SDL_stdinc.h>`

**Path Aliases:**
- No path aliases detected. All includes use relative or direct file paths
- Relative paths with `./` and `../` used to navigate directory structure

## Error Handling

**Patterns:**
- Division by zero checks in critical calculations:
  - `Vec2::Normalize()`: Checks `if (length != 0.0)` before dividing
  - `Vec2::UnitVector()`: Same safety check for zero-length vectors
- Null pointer checks in destructors: `if (particle != nullptr)` before deletion
- Distance clamping in physics calculations: `std::clamp(direction.MagnitudeSquared(), minDistance, maxDistance)` in `Force::GenerateGravitationalForce()`
- No formal exception handling or try-catch blocks observed
- Silent failures in some cases (e.g., `ComputeDistanceField()` returns `0.0f` stub)

## Logging

**Framework:** `std::cout` for debug output only

**Patterns (observed):**
- Constructor/destructor logging: `std::cout << "2D Particle created with mass: " << mass << std::endl;`
- System lifecycle: `std::cout << "System Initiated" << std::endl;` and `std::cout << "System Destroyed" << std::endl;`
- Logging lines are commented out or not present in many files
- No structured logging; no log levels (info, warn, error)
- Single-line output format

## Comments

**When to Comment:**
- Block headers identifying major sections: `// ---- Force Calculations ----`, `// ---- Operator Overloading ----`
- Complex physics calculations get inline comments: Described in Force.cpp implementations
- TODOs mark incomplete or uncertain implementations
- Commented-out code preserved (debug/old implementations)

**JSDoc/TSDoc:**
- C++ style block comments used for function documentation: `/** @brief [description] */`
- Parameters documented with `@param` tags
- Return values documented with `@return` tags
- Example from `Force::GenerateDragForce()`:
  ```cpp
  /**
   * @brief Generates a drag force on a particle
   *
   * @param particle The particle to generate the force on
   * @param dragCoefficient The drag coefficient of the particle
   * @param area The area of the particle
   *
   * @return Vec2 The drag force
   */
  ```
- Not all functions have documentation; primarily physics/force functions documented

## Function Design

**Size:**
- Functions generally short to medium length (10-50 lines typical)
- Longest functions are integration methods (RK4: ~30 lines) and collision resolution (~50 lines)
- Force generation functions ~20 lines average

**Parameters:**
- Pass by const reference for efficiency: `const Vec2& force`, `const Particle2D& particle`, `const std::vector<Particle2D*>& neighbors`
- Raw pointers used for dynamic objects: `Particle2D* other`, `Spring* spring`
- float for numeric parameters: `float dt`, `float mass`, `float radius`

**Return Values:**
- Return by value for small types: `Vec2`, structs (Vec2 + operator results)
- Return primitive types: `float`, `double`, `bool`
- Mutable reference return for chainable operations: `Vec2& Normalize()` returns `*this`

## Module Design

**Exports:**
- Struct/class definitions in headers, implementations in `.cpp` files
- Public headers expose full interface (no private headers)
- Static utility functions in Force struct for decoupled force calculations

**Barrel Files:**
- No barrel files pattern detected
- Each class/struct has its own header-implementation pair
- Direct includes required for each dependency

## Build Integration

**Build System:** Makefile with single command
- Build command: `g++ -std=c++17 -Wall ./src/*.cpp ./src/Physics/*.cpp ./src/Inputs/*.cpp ./src/Scenes/*.cpp ./src/Graphics/*.cpp -lm -lSDL2 -lSDL2_gfx -o engine`
- No incremental compilation; rebuilds all files
- All source directories explicitly listed in compilation

---

*Convention analysis: 2026-01-23*
