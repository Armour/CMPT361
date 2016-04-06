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
    Triangle() {};
    ~Triangle() {};

    inline void set_v1(glm::vec3 v1) { this->v1 = v1; };
    inline void set_v2(glm::vec3 v2) { this->v2 = v2; };
    inline void set_v3(glm::vec3 v3) { this->v3 = v3; };

    glm::vec3 GetNormal(glm::vec3 dummy);       // Get planer surface normal
    float IntersectRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 *hit);      // Intersect with ray
    bool InCubeRange(glm::vec3 min_pos, glm::vec3 max_pos);         // Check if a triangle is in a specific cube range

    bool InTriangle(glm::vec3 point);           // Check if the hit point in the triangle area
};

// Add a triangle to the object list
Object *AddTriangle(Object *objects, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                    float shininess, float reflectance, float refractance, float refract_ratio, int index, bool infinite);

// Test if a plane is overlap with an AABB box
bool PlaneBoxOverlap(glm::vec3 normal, glm::vec3 vertex, glm::vec3 box_size);

}  // namespace raychess

#endif //RAYCHESS_TRIANGLE_H_
