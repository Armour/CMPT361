# ASSIGNMENT 1
#### Chong Guo armourg@sfu.ca

## Not Finished Features

- None

## File Structure

- src -- the source file folder

    - fruit_tetris -- Main function, OpenGL related function
    - game_manager.cc -- Elegant game manager with game states
    - init_shader.cc -- Used to read shader

- include -- the header file folder

    - lib_consts.h -- Constant variables header
    - lib_gl.h -- Include GL related header
    - init_shader.h -- Used to read shader
    - game_manager.h -- Elegant game manager with game states
    - glm -- OpenGL math library

- cmake -- the cmake findmodule file folder
 
    
## How to use

- Mac
    - Use `Clion`
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - `./FruitTetris` 

- Ubuntu
    - Use `Clion`
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - `./FruitTetris` 

- If `make` in ubuntu not well, you should remove depend and try to run `make` twice.

## Additional Features

- Functions
    - Four different game mode by press key `1`, `2`, `3`, `4`
    - Press `p` to pause the game and again to resume 

- Others
    - Cross platform (Clion + Cmake)
    - Following Google C++ Style, well commented
    - Self-write almost all the skeleton code with comment
    - Put all constant values to one header file, elegant and easy to modify
