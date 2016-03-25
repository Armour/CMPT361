//////////////////////////////////////////////////////////////////////////////
//
//  triangle.h
//
//  The source file for triangle class
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 3/24/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "triangle.h"
#include <iostream>

namespace raychess {

//
// Function: IntersectRay
// ---------------------------
//
//   This function intersects a ray with a triangle
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

float Triangle::IntersectRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 *hit) {
    glm::vec3 norm = Normal(glm::vec3(0.0f));
    float t = -(glm::dot(norm, origin) - glm::dot(norm, v1)) / glm::dot(norm, direction);
    if (t < 0) return -1;
    *hit = origin + t * direction;
    if (!InTriangle(*hit)) return -1;
    return t;
}

//
// Function: Normal
// ---------------------------
//
//   Return the unit normal at a point on the triangle
//
//   Parameters:
//       dummy: useless point
//
//   Returns:
//       triangle normal vector
//

glm::vec3 Triangle::Normal(glm::vec3 dummy) {
    glm::vec3 u = v1 - v3;
    glm::vec3 v = v2 - v3;
    return glm::normalize(glm::cross(u, v));
}

//
// Function: AddTriangle
// ---------------------------
//
//   This function adds a triangle into the object list
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

Object *AddTriangle(Object *objects, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                    float shininess, float reflectance, float refractance, float refract_ratio, int index) {
    Triangle *new_triangle;
    new_triangle = (Triangle *)malloc(sizeof(Triangle));
    new_triangle->set_index(index);
    new_triangle->set_type(libconsts::kTypeTriangle);
    new_triangle->set_v1(v1);
    new_triangle->set_v2(v2);
    new_triangle->set_v3(v3);
    new_triangle->set_ambient(ambient);
    new_triangle->set_diffuse(diffuse);
    new_triangle->set_specular(specular);
    new_triangle->set_shininess(shininess);
    new_triangle->set_reflectance(reflectance);
    new_triangle->set_refractance(refractance);
    new_triangle->set_refract_ratio(refract_ratio);
    new_triangle->set_next(nullptr);

    if (objects == nullptr) {           // First object
        objects = (Object *)new_triangle;
    } else {                            // Insert at the beginning
        new_triangle->set_next(objects);
        objects = (Object *)new_triangle;
    }

    return objects;
}

//
// Function: InTriangle
// ---------------------------
//
//   Check if the hit point in the triangle area
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

bool Triangle::InTriangle(glm::vec3 hit) {
    glm::vec3 t1 = glm::cross(v1 - v3, v2 - v3);
    glm::vec3 t2 = glm::cross(v1 - hit, v2 - hit);

    if (t1.x * t2.x < 0 || t1.y * t2.y < 0 || t1.z * t2.z < 0) {
        return false;
    }

    t2 = glm::cross(v2 - hit, v3 - hit);

    if (t1.x * t2.x < 0 || t1.y * t2.y < 0 || t1.z * t2.z < 0) {
        return false;
    }

    t2 = glm::cross(v3 - hit, v1 - hit);

    if (t1.x * t2.x < 0 || t1.y * t2.y < 0 || t1.z * t2.z < 0) {
        return false;
    }
    return true;
}

}  // namespace raychess

