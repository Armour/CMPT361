# ASSIGNMENT 3
#### Chong Guo armourg@sfu.ca

## Not Finished Features

- None
- **Bonus Added**

## File Structure

- src -- the source file folder
    - image_util.cc -- Used for image manipulation
    - init_shader.cc -- Used to read shader
    - **octree.cc** -- Used to reduce intersection test
    - object.cc -- The base class for object in scene
    - sphere.cc -- The derived class from object
    - triangle.cc -- The derived class from object
    - scene.cc -- Used for set up default or user defined scene
    - trace.cc -- Contains the function about recursive ray tracing
    - raycast.cc -- The main source file contain OpenGL set up

- include -- the header file folder
    - image_util.h -- Used for image manipulation
    - init_shader.h -- Used to read shader
    - lib_consts.h -- Constant variables header
    - lib_gl.h -- Include GL related header
    - **octree.h** -- Used to reduce intersection test
    - object.h -- The base class for object in scene
    - sphere.h -- The derived class from object
    - triangle.h -- The derived class from object
    - scene.h -- Used for set up default or user defined scene
    - trace.h -- Contains the function about recursive ray tracing
    - glm -- OpenGL math library

- cmake -- the cmake findmodule file folder
    - FindGLUI.cmake -- Used to find GLUI in different platform
    
## How to use

- Mac
    - Use `Clion`
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - Use `./raycast` to run basic part
        - Use `./raychess` to run bonus part
        - (PS: Actually they are the same lol)

- Ubuntu
    - Use `Clion`
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - Use `./raycast` to run basic part
        - Use `./raychess` to run bonus part
        - (PS: Actually they are the same lol)

## Additional Features

- Functions
    - **Parallel programming** (OpenMP)
    - **Octree** implemented, use `+o` to open it
    - OOP with **Polymorphism**
    
- Others
    - Cross platform (Clion + Cmake)
    - Following Google C++ Style, well commented
    - Self-write almost all the skeleton code with comment
    - Put all constant values to one header file, elegant and easy to modify
