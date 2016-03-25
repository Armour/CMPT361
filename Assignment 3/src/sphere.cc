//////////////////////////////////////////////////////////////////////////////
//
//  sphere.h
//
//  The source file for sphere class
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

#include "sphere.h"

namespace raychess {

//
// Function: IntersectRay
// ---------------------------
//
//   This function intersects a ray with a sphere
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

float Sphere::IntersectRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 *hit) {
    float a = powf((float)glm::length(direction), 2);
    float b = 2 * (float)glm::dot(direction, origin - this->get_center());
    float c = powf((float)glm::length(origin - this->get_center()), 2) - powf(this->get_radius(), 2);
    float delta = powf(b, 2) - 4 * a * c;
    if (delta < 0) return -1;

    float t1 = (-b - sqrtf(delta)) / 2 / a;
    float t2 = (-b + sqrtf(delta)) / 2 / a;
    if (t1 < 0 && t2 < 0) return -1;

    glm::vec3 p2 = origin + direction * t2;
    glm::vec3 p1 = origin + direction * t1;
    *hit = t1 <= t2? p1: p2;
    return t1 <= t2? t1: t2;
}

//
// Function: Normal
// ---------------------------
//
//   Return the unit normal at a point on the sphere
//
//   Parameters:
//       surf_point: a point on the sphere surface
//
//   Returns:
//       sphere normal vector
//

glm::vec3 Sphere::Normal(glm::vec3 surf_point) {
    return glm::normalize(surf_point - this->get_center());
}

//
// Function: AddSphere
// ---------------------------
//
//   This function adds a sphere into the object list
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

Object *AddSphere(Object *objects, glm::vec3 center, float radius, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                  float shininess, float reflectance, float refractance, float refract_ratio, int index) {
    Sphere *new_sphere;
    new_sphere = (Sphere *)malloc(sizeof(Sphere));
    new_sphere->set_index(index);
    new_sphere->set_type(libconsts::kTypeSphere);
    new_sphere->set_center(center);
    new_sphere->set_radius(radius);
    new_sphere->set_ambient(ambient);
    new_sphere->set_diffuse(diffuse);
    new_sphere->set_specular(specular);
    new_sphere->set_shininess(shininess);
    new_sphere->set_reflectance(reflectance);
    new_sphere->set_refractance(refractance);
    new_sphere->set_refract_ratio(refract_ratio);
    new_sphere->set_next(nullptr);

    if (objects == nullptr) {           // First object
        objects = (Object *)new_sphere;
    } else {                            // Insert at the beginning
        new_sphere->set_next(objects);
        objects = (Object *)new_sphere;
    }

    return objects;
}

}  // namespace raychess
