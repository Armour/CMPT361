//////////////////////////////////////////////////////////////////////////////
//
//  sphere.h
//
//  The header file to handle spheres
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

#ifndef RAYCHESS_SPHERE_H_
#define RAYCHESS_SPHERE_H_

#include "object.h"
#include "lib_gl.h"
#include "lib_consts.h"

namespace raychess {

class Sphere : public Object {
private:
    float radius_;               // The radius of this sphere
    glm::vec3 center_;           // The center point of this sphere
public:
    inline float get_radius(void) const { return radius_; };
    inline glm::vec3 get_center(void) const { return center_; };
    inline void set_radius(float radius) { this->radius_ = radius; };
    inline void set_center(glm::vec3 center) { this->center_ = center; };
};

// Intersect ray with sphere
float IntersectSphere(glm::vec3 origin, glm::vec3 direction, Sphere *sphere, glm::vec3 *hit);

// Intersect ray with scene
Object *IntersectScene(glm::vec3 origin, glm::vec3 direction, Object *objects, glm::vec3 *hit, int sphere_ignore);

// Add a sphere to the object list
Object *AddSphere(Object *objects, glm::vec3 center, float radius, glm::vec3 ambient, glm::vec3 diffuse,
                  glm::vec3 specular, float shininess, float reflectance, int index);

// Return the unit normal at a point on sphere
glm::vec3 SphereNormal(glm::vec3 surf_point, Sphere *sphere);

}  // namespace raychess

#endif  // RAYCHESS_SPHERE_H_
