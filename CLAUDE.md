# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build and Development Commands

### Building the project
```bash
make build
```
This compiles all source files using g++ with C++17 standard and links against SDL2, SDL2_image, and SDL2_gfx libraries.

### Running the engine
```bash
make run
# or directly:
./engine
```

### Cleaning build artifacts
```bash
make clean
```

## Project Architecture

This is a 2D/3D physics engine written in C++ for interactive particle simulations and fluid dynamics research.

### Core Application Structure
- **Main.cpp**: Entry point with standard game loop (Setup → Input → Update → Render → Destroy)
- **Application**: Main application class managing the overall system state and scene switching
- **System**: Base class for simulation systems with configurable physics toggles
- **Scenes**: Specific simulation implementations (SpringForceLattice, WaterDam)

### Physics Engine Components

#### Core Physics Classes
- **Particle2D/3D**: Point masses with position, velocity, acceleration, mass, radius, and color
- **Vec2/Vec3**: Vector mathematics for 2D and 3D operations
- **Force**: Comprehensive force calculation system including:
  - Gravitational forces (planetary and inter-particle)
  - Friction and drag forces
  - Spring forces for connected particles
- **Spring**: Connects particles with spring physics
- **Octree2D**: Spatial partitioning for efficient neighbor finding and collision detection

#### Advanced Features
- **Multiple Integration Methods**: Runge-Kutta 4th Order (preferred), Euler, and Verlet
- **SPH Implementation**: Smoothed Particle Hydrodynamics in development
- **Adaptive Grid**: Future feature for variable resolution SPH simulation

### System Configuration
The System class provides runtime toggles for all major physics components:
- Integration, fluid dynamics, gravity, attraction, friction, drag
- Particle and screen collision detection
- Spring forces
- Debug features (dev menu, mouse/keyboard input)

### Graphics and Input
- **Graphics**: SDL2-based rendering system
- **Mouse**: Mouse input handling for particle creation and interaction
- Particle creation via left-click at mouse position

### Constants and Configuration
- **Constants.hpp**: Centralized physics constants including:
  - FPS (60), frame timing
  - PIXELS_PER_METER (4000) for world scaling
  - Physics constants (gravity, gravitational constant, fluid properties)

## Dependencies
- SDL2 (graphics)
- SDL2_image (image loading)
- SDL2_gfx (graphics utilities)
- C++17 compatible compiler

## Current Development Focus
The project is actively developing SPH (Smoothed Particle Hydrodynamics) using PIC/FLIP methods with adaptive resolution capabilities. See TODO file for specific implementation tasks around Gaussian kernel functions.