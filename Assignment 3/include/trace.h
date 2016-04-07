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
#include "object.h"
#include "raycast.h"

namespace raychess {

// The phong illumination implementation
glm::vec3 PhongIllumination(RenderManager *manager, Object *object, glm::vec3 hit, glm::vec3 surf_norm, bool shadow_on);

// This is the recursive ray trace function
glm::vec3 RecursiveRayTrace(RenderManager *manager, glm::vec3 origin, glm::vec3 direction, int depth, int object_ignore);

// This function do the ray trace, using the recursive ray trace above for every pixels
void RayTrace(RenderManager *manager);

// Intersect ray with objects in scene
Object *IntersectScene(RenderManager *manager, glm::vec3 origin, glm::vec3 direction, Object *objects, glm::vec3 *hit, int object_ignore);

}  // namespace raychess

#endif  // RAYCHESS_RAY_TRACE_H_
