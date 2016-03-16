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

#include "sphere.h"
#include "lib_consts.h"

namespace raychess {

// The phong illumination implementation
glm::vec3 PhongIllumination(glm::vec3 q, glm::vec3 v, glm::vec3 surf_norm, raychess::Sphere *sph);

// This is the recursive ray trace function
glm::vec3 RecursiveRayTrace();

// This function do the ray trace, using the recursive ray trace above for every pixels
void RayTrace();

}  // namespace raychess

#endif  // RAYCHESS_RAY_TRACE_H_
