# PhysicsEngine
A Physics Engine I am writing for art projects

# Currently Implemented Features
- Particles represent point masses and could have a mass, radius, colour.
- Force Calculations for Point Masses: Gravity (Between Particles) Gravity ("Downward Facding, simulating gravity on Earth"), Friction, Drag & some more.
- Euler and Verlet Integration (Interchangeable, but Verlet is preferred so far).
- Point Mass Collision Detection & Resolution existis (+ Fixed Clipping Issue when a point mass is spawned inside a point mass).
- Simulation Flags to toggle certain forces/interactions are present, but not all of them are mapped to a keyboard
- Creates a single particle at a mouse position with left click.
- Graphics are implemented through SDL, but that is not a final decision as of now.


# Running Example
https://user-images.githubusercontent.com/30289989/206562822-e5b55257-bf9a-496b-99cf-b3822103a13d.mov



# DISCLAIMER
- I am not responsible if you OR I break your computer.
- I might be underqualified to explain anything that I am doing.
- Proceed at your own risk, you have been warned.


# How to run
1. You need SDL2. You need a c++ compiler.
2. Copy into the folder, run the make file with $make
3. run the generated ./engine file
VOILA. You are ready to run.

# Future Features
- I am working on implementing an SPH (Smoothed Praticle Hydrodynamics) using PIC/FLIP method.
- I am also trying to implement Adaptive Grid Size for the SPH simulation.

# References 
- Adaptive resolution for multiphase smoothed particle hydrodynamics by Xiufeng Yang, Song-Charng Kong [Research DOI](https://doi.org/10.1016/j.cpc.2019.01.002)


