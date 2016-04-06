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
//       origin: the origin point of the ray
//       direction: the direction of the ray
//       hit: the intersection point, nullptr if not intersect
//
//   Returns:
//       the length from origin to hit point
//

float Sphere::IntersectRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 *hit) {
    float a = powf((float)glm::length(direction), 2);
    float b = 2 * (float)glm::dot(direction, origin - center_);
    float c = powf((float)glm::length(origin - center_), 2) - powf(radius_, 2);
    float delta = powf(b, 2) - 4 * a * c;
    if (delta < 0) return -1;

    float t1 = (-b - sqrtf(delta)) / 2 / a;
    float t2 = (-b + sqrtf(delta)) / 2 / a;
    if (t1 < 0 && t2 < 0) return -1;

    glm::vec3 p2 = origin + direction * t2;
    glm::vec3 p1 = origin + direction * t1;
    *hit = t1 <= t2 && t1 >= 0? p1: p2;
    return t1 <= t2 && t1 >= 0? t1: t2;
}

//
// Function: GetNormal
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

glm::vec3 Sphere::GetNormal(glm::vec3 surf_point) {
    return glm::normalize(surf_point - center_);
}

//
// Function: AddSphere
// ---------------------------
//
//   This function adds a sphere into the object list
//
//   Parameters:
//       the parameters for the sphere
//
//   Returns:
//       the pointer to the object list
//

Object *AddSphere(Object *objects, glm::vec3 center, float radius, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                  float shininess, float reflectance, float refractance, float refract_ratio, int index) {
    Sphere *new_sphere = new Sphere();
    new_sphere->set_index(index);
    new_sphere->set_center(center);
    new_sphere->set_radius(radius);
    new_sphere->set_ambient(ambient);
    new_sphere->set_diffuse(diffuse);
    new_sphere->set_specular(specular);
    new_sphere->set_shininess(shininess);
    new_sphere->set_reflectance(reflectance);
    new_sphere->set_refractance(refractance);
    new_sphere->set_refract_ratio(refract_ratio);
    new_sphere->set_infinite(false);
    new_sphere->set_next(nullptr);

    Object *object = new_sphere;
    if (objects == nullptr) {           // First object
        objects = object;
    } else {                            // Insert at the beginning
        object->set_next(objects);
        objects = object;
    }

    return objects;
}

//
// Function: InCubeRange
// ---------------------------
//   Check if a sphere is in a specific cube range
//
//   Parameters:
//       min_pos: the minimum position of the cube
//       max_pos: the maximum position of the cube
//
//   Returns:
//       if the sphere is in the cube
//

bool Sphere::InCubeRange(glm::vec3 min_pos, glm::vec3 max_pos) {
    float dist_squared = powf(radius_, 2);
    if (center_.x < min_pos.x) dist_squared -= powf(center_.x - min_pos.x, 2);
        else if (center_.x > max_pos.x) dist_squared -= powf(center_.x - max_pos.x, 2);
    if (center_.y < min_pos.y) dist_squared -= powf(center_.y - min_pos.y, 2);
        else if (center_.y > max_pos.y) dist_squared -= powf(center_.y - max_pos.y, 2);
    if (center_.z < min_pos.z) dist_squared -= powf(center_.z - min_pos.z, 2);
        else if (center_.z > max_pos.z) dist_squared -= powf(center_.z - max_pos.z, 2);
    return dist_squared > 0;
}

}  // namespace raychess
