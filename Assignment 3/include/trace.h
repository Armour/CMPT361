//////////////////////////////////////////////////////////////////////////////
//
//  trace.h
//
//  The header file for ray trace function
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

#ifndef RAYCHESS_RAY_TRACE_H_
#define RAYCHESS_RAY_TRACE_H_

#include "lib_consts.h"
#include "sphere.h"
#include "triangle.h"

namespace raychess {

// The phong illumination implementation
glm::vec3 PhongIllumination(Object *object, glm::vec3 hit, glm::vec3 surf_norm);

// This is the recursive ray trace function
glm::vec3 RecursiveRayTrace(glm::vec3 origin, glm::vec3 direction, int iteration, int sphere_ignore, bool in_object);

// This function do the ray trace, using the recursive ray trace above for every pixels
void RayTrace(int iteration);

}  // namespace raychess

#endif  // RAYCHESS_RAY_TRACE_H_
