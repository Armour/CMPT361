//////////////////////////////////////////////////////////////////////////////
//
//  sphere.h
//
//  The source file to handle spheres
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
#include <iostream>

namespace raychess {

//
// Function: IntersectSphere
// ---------------------------
//
//   This function intersects a ray with a given sphere 'sph'. You should
//   use the parametric representation of a line and do the intersection.
//   The function should return the parameter value for the intersection,
//   which will be compared with others to determine which intersection
//   is closest. The value -1.0 is returned if there is no intersection
//   If there is an intersection, the point of intersection should be
//   stored in the "hit" variable
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

float IntersectSphere(glm::vec3 origin, glm::vec3 direction, raychess::Sphere *sphere, glm::vec3 *hit) {
    float a = powf((float)glm::length(direction), 2);
    float b = 2 * (float)glm::dot(direction, origin - sphere->center);
    float c = powf((float)glm::length(origin - sphere->center), 2) - powf(sphere->radius, 2);
    float delta = powf(b, 2) - 4 * a * c;
    if (delta < 0) return -1;

    float t1 = (-b - sqrtf(delta)) / 2 / a;
    float t2 = (-b + sqrtf(delta)) / 2 / a;
    if (t1 < 0 && t2 < 0) {
        return -1;
    }

    glm::vec3 p2 = origin + direction * t2;
    glm::vec3 p1 = origin + direction * t1;
    if ((p1 - origin).length() <= (p2 - origin).length()) {
        *hit = p1;
        return t1;
    } else {
        *hit = p2;
        return t2;
    }
}

//
// Function: IntersectScene
// ---------------------------
//
//   Decide the intersection of ray and scene objects
//
//   Parameters:
//       origin: the origin point of ray
//       direction: the direction vector of ray
//       spheres: the sphere list (scene objects)
//       max_distance: the max distance we can accept
//       hit: the intersection point
//       sphere_ignore: the sphere index that we need to ignore
//
//   Returns:
//       A pointer to the sphere object that the ray intersects first, nullptr if no intersection.
//

raychess::Sphere *IntersectScene(glm::vec3 origin, glm::vec3 direction, Sphere *spheres,
                                 float max_distance, glm::vec3 *hit, int sphere_ignore) {
    Sphere *result = nullptr;
    Sphere *current_sphere = spheres;
    glm::vec3 *current_hit = new glm::vec3();
    while (current_sphere != nullptr) {
        if (current_sphere->index != sphere_ignore) {
            float distance = IntersectSphere(origin, direction, current_sphere, current_hit);
            if (distance != -1 && distance < max_distance) {
                if (result == nullptr || (*hit - origin).length() > (*current_hit - origin).length()) {
                    *hit = *current_hit;
                    result = current_sphere;
                }
            }
        }
        current_sphere = current_sphere->next;
    }
    delete current_hit;
    return result;
}

//
// Function: AddSphere
// ---------------------------
//
//   This function adds a sphere into the sphere list
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

raychess::Sphere *AddSphere(raychess::Sphere *spheres, glm::vec3 center, float radius, glm::vec3 ambient,
                            glm::vec3 diffuse, glm::vec3 specular, float shineness, float reflectance, int index) {
    raychess::Sphere *new_sphere;
    new_sphere = (raychess::Sphere *)malloc(sizeof(raychess::Sphere));
    new_sphere->index = index;
    new_sphere->center = center;
    new_sphere->radius = radius;
    (new_sphere->mat_ambient) = ambient;
    (new_sphere->mat_diffuse) = diffuse;
    (new_sphere->mat_specular) = specular;
    new_sphere->mat_shininess = shineness;
    new_sphere->reflectance = reflectance;
    new_sphere->next = NULL;

    if (spheres == NULL) {          // First object
        spheres = new_sphere;
    } else {                        // Insert at the beginning
        new_sphere->next = spheres;
        spheres = new_sphere;
    }

    return spheres;
}

//
// Function: SphereNormal
// ---------------------------
//
//   Return the unit normal at a point on the sphere
//
//   Parameters:
//       surf_point: a point on the sphere surface
//       sphere: the sphere we used to compute normal
//
//   Returns:
//       sphere normal vector
//

glm::vec3 SphereNormal(glm::vec3 surf_point, raychess::Sphere *sphere) {
    return glm::normalize(surf_point - sphere->center);
}

/*
glm::vec3 PlanerNormal(raychess::Planer *planer) {
    glm::vec3 u = planar_points[planer->a] - planar_points[planer->c];
    glm::vec3 v = planar_points[planer->b] - planar_points[planer->c];
    return glm::cross(u, v);
}
*/

}  // namespace raychess
