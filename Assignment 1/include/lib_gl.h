//////////////////////////////////////////////////////////////////////////////
//
//  lib_gl.h
//
//  The header file for include OpenGL headers in different operating systems
//
//  Project         : FruitTetris
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 2/3/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef FRUITTETRIS_LIB_GL_H_
#define FRUITTETRIS_LIB_GL_H_

// Include OpenGL relevant headers
#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include <OpenGL/gl3.h>
#  include "glm/glm.hpp"
#else   // End of Mac OSX header include
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#  include "glm/glm.hpp"
#endif  // End of Linux header include

#endif //FRUITTETRIS_LIB_GL_H_
