//////////////////////////////////////////////////////////////////////////////
//
//  lib_gl.h
//
//  The header file for include OpenGL headers in different operating systems
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 3/15/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef RAYCHESS_LIB_GL_H_
#define RAYCHESS_LIB_GL_H_

// Include OpenGL relevant headers
#ifdef __APPLE__
#  include <GL/glui.h>
#  include <GLUT/glut.h>
#  include <OpenGL/glu.h>
#  include <OpenGL/gl3.h>
#  include <glm/glm.hpp>
#  include <glm/gtx/component_wise.hpp>
#else   // End of Mac OSX header include
#  include <GL/glew.h>
#  include <GL/glui.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#  include <glm/glm.hpp>
#  include <glm/gtx/component_wise.hpp>
#endif  // End of Linux header include

#endif  //RAYCHESS_LIB_GL_H_
