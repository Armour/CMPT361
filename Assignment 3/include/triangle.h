//////////////////////////////////////////////////////////////////////////////
//
//  triangle.h
//
//  The header file for triangle class
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 3/23/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef RAYCHESS_TRIANGLE_H_
#define RAYCHESS_TRIANGLE_H_

#include "object.h"

namespace raychess {

class Triangle : public Object {
private:
    glm::vec3 v1, v2, v3;               // The three vertex of this triangle
public:
    inline void set_v1(glm::vec3 v1) { this->v1 = v1; };
    inline void set_v2(glm::vec3 v2) { this->v2 = v2; };
    inline void set_v3(glm::vec3 v3) { this->v3 = v3; };
    glm::vec3 Normal(glm::vec3 dummy);          // Get planer surface normal
    float IntersectRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 *hit);      // Intersect with ray
    bool InTriangle(glm::vec3);         // Check if the hit point in the triangle area
};

// Add a triangle to the object list
Object *AddTriangle(Object *objects, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                    float shininess, float reflectance, float refractance, float refract_ratio, int index);

}  // namespace raychess

#endif //RAYCHESS_TRIANGLE_H_
