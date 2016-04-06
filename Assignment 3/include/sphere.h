//////////////////////////////////////////////////////////////////////////////
//
//  sphere.h
//
//  The header file for sphere class
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
    Sphere() {};
    ~Sphere() {};
    inline float get_radius(void) const { return radius_; };
    inline glm::vec3 get_center(void) const { return center_; };
    inline void set_radius(float radius) { this->radius_ = radius; };
    inline void set_center(glm::vec3 center) { this->center_ = center; };
    glm::vec3 Normal(glm::vec3 surf_point);                                         // Get sphere surface normal
    float IntersectRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 *hit);      // Intersect with ray
    bool InCubeRange(glm::vec3 min_pos, glm::vec3 max_pos);         // Check if a sphere is in a specific cube range
};

// Add a sphere to the object list
Object *AddSphere(Object *objects, glm::vec3 center, float radius, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                  float shininess, float reflectance, float refractance, float refract_ratio, int index);

}  // namespace raychess

#endif  // RAYCHESS_SPHERE_H_
