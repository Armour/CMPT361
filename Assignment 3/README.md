# ASSIGNMENT 3
#### Chong Guo armourg@sfu.ca

## Not Finished Features

- None
- Bonus Added

## File Structure

- src -- the source file folder
    - init_shader.cc -- Used to read shader

- include -- the header file folder
    - init_shader.h -- Used to read shader
    - lib_consts.h -- Constant variables header
    - lib_gl.h -- Include GL related header
    - glm -- OpenGL math library

- cmake -- the cmake findmodule file folder
    - FindGLUI.cmake -- Used to find GLUI in different platform
    
## How to use

- Mac
    - Use `Clion`
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - `./raychess` 

- Ubuntu
    - Use `Clion`
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - `./raychess` 

- If `make` in ubuntu failed, you should remove depend and try to run `make` twice.

## Additional Features

- Functions

- Others
    - Cross platform (Clion + Cmake)
    - Following Google C++ Style, well commented
    - Self-write almost all the skeleton code with comment
    - Put all constant values to one header file, elegant and easy to modify
