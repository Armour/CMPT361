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
        - `./raychess`

- Ubuntu
    - Use `Clion`
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - `./raychess`

## Features

- Speed Up
    - **Octree** implemented, use `+o` to enable it	

	- For a single chess piece in the chessboard:
	    - Without Octree:
	        - Time: 13.05s
	        - Intersection number: 211523500
	    - With Octree:
	        - Time: 5.38s
	        - Intersection number: 14265930

	- For two chess pieces in the chessboard:
	    - Without Octree:
	        - Time: 201.32s
	        - Intersection number: 3355443200
	    - With Octree:
	        - Time: 19.13s
	        - Intersection number: 39631770

	- Comment:
	    - Reduce lots of unnecessary intersection tests (>= 90%)

    - **Parallel programming** added (using OpenMP)

	- For two chess pieces in the chessboard:
	    - Without OpenMP:
	        - Time: 201.32s
	        - Intersection number: 3355443200
	    - With OpenMP:
	        - Time: 83.78s
	        - Intersection number: 3355443200

	- Comment:
	    - Doesn't reduce the intersection test number, but saved more than half time.
    
- Others
    - OOP with **Polymorphism**
    - Cross platform (Clion + Cmake)
    - Following Google C++ Style, well commented
    - Self-write almost all the skeleton code with comment
    - Put all constant values to one header file, elegant and easy to modify

## Result

- See images in `results` folder
    - Four images with different meterial parameters set (one with refraction ratio about 1.0f)
