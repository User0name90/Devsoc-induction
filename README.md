# Physics Engine
This is regular 2-D physics engine for demonstration. You can only make circles. You can turn ON gravity.
The most fun feature is snowball effect.
## Features
### Multiple spheres
You can add as many spheres as you want until the engine becomes slow. The spheres that will be added will have variable radius and initial velocity. The color of sphere depends on their density. A blue sphere is least dense followed by green and finally red is most dense.
### Audio
Sound of each collision depends on impulse of collision (It just increases volume with increase in impulse).
### Gravity
You can add gravity to the engine. In order to change its magnitude, go to line number 98 in [src/Center.cpp](src/Center.cpp)
```cpp
eg.setGravity({0,60});
```
Change y value(second parameter) to desired value. You can also change x value if you want diagonal gravity :)
### Debug
It is a lazy feature that adds a common tangent to circles at collision at point of contact
To change the length and breadth of line(actually rectangle), go to line number 4 and 5 in [include/engine.cpp](include/engine.cpp)
```cpp
debug_line.setSize({60.f,3.f});
debug_line.setOrigin({30.f,1.5f});
```
To change the length, change the 1st parameter in setSize() and corresponding change the 1st parameter in setOrigin() to half of the length that you entered. Do same for breadth
### Snowball
This feature took a lot of time and my sanity. The size of circles decreases with each collision and you can see particles flying off. Particles are also effected by gravity( hard to see since their max lifetime is 2 sec). The velocity of spheres also reduces as compared to elastic collisions. 
To change number of particles produced per unit loss of mass. Go to line 32 in [include/splasher_engine.hpp](include/splasher_engine.hpp) 
```cpp
const unsigned int mass_to_particle=100;
```
and change its value
Note:- Changing reduction factor in line 31 changes the how much mass of the sphere will reduce as well as its velocity.
In short, the loss of energy is proportional the to square of reduction_factor.

### Warning
In snowball mode, each splash create a new thread. If you turn on snowball mode when there are too many spheres. the program will stall as it will not get required number of threads to execute.
## Requirements
Minimum CMake version is 3.28
Minimum C++ standard required is C++20. (install a newer version of gcc if the current one does not support, preferably gcc 13 or greater) 
It runs on linux and mac only. (Tell me if you want to run it on windows)

## How to run
in the directory where you copied it, run:
```bash
cmake -S . -B build
cmake --build build
build/bin/main
```
there is no `main.cpp` file between.

## Additional
If you want me to add comments, just ask AI to do it.
