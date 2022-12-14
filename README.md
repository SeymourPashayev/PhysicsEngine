# PhysicsEngine
A Physics Engine I am writing for art projects

# Currently Implemented Features
- Particles represent point masses and could have a mass, radius, colour.
- Force Calculations for Point Masses: Gravity (Between Particles) Gravity ("Downward Facing, simulating gravity on Earth"), Friction, Drag & some more.
- Runge-Kutta 4th Order, Euler and Verlet Integration Methods (Interchangeable, but RK4 is preferred).
- Octree Data Structure to decrease the computational overhead of finding neighbouring particles.
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
- Adaptive resolution for multiphase smoothed particle hydrodynamics by Xiufeng Yang, Song-Charng Kong [DOI](https://doi.org/10.1016/j.cpc.2019.01.002)
- Real-time fluid simulation with Adaptive SPH by By He Yan, Zhangye Wang, Jian He, Xi Chen, Changbo Wang and Qunsheng Peng [DOI](https://doi.org/10.1002/cav.300)
- Fluid Simulation for Computer Graphics (Second Edition) by Robert Bridson [ISBN-13: 978-148223283]


