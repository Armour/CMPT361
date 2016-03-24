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

float IntersectSphere(glm::vec3 origin, glm::vec3 direction, Sphere *sphere, glm::vec3 *hit) {
    float a = powf((float)glm::length(direction), 2);
    float b = 2 * (float)glm::dot(direction, origin - sphere->get_center());
    float c = powf((float)glm::length(origin - sphere->get_center()), 2) - powf(sphere->get_radius(), 2);
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

Object *IntersectScene(glm::vec3 origin, glm::vec3 direction, Object *objects, glm::vec3 *hit, int sphere_ignore) {
    Object *result = nullptr;
    Object *current_object = objects;
    glm::vec3 *current_hit = new glm::vec3();
    while (current_object != nullptr) {
        //if (current_object->get_index() != sphere_ignore) {
        if (current_object->get_type() == libconsts::kTypeSphere) {
            float distance = IntersectSphere(origin, direction, (Sphere *)current_object, current_hit);
            if (distance != -1 && distance < libconsts::kMaxDistance) {
                if (result == nullptr || (*hit - origin).length() > (*current_hit - origin).length()) {
                    *hit = *current_hit;
                    result = current_object;
                }
            }
        }
        //}
        current_object = current_object->get_next();
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

Object *AddSphere(Object *objects, glm::vec3 center, float radius, glm::vec3 ambient, glm::vec3 diffuse,
                  glm::vec3 specular, float shininess, float reflectance, int index) {
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
    new_sphere->set_next(nullptr);

    if (objects == nullptr) {          // First object
        objects = (Object *)new_sphere;
    } else {                        // Insert at the beginning
        new_sphere->set_next(objects);
        objects = (Object *)new_sphere;
    }

    return objects;
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

glm::vec3 SphereNormal(glm::vec3 surf_point, Sphere *sphere) {
    return glm::normalize(surf_point - sphere->get_center());
}

/*
glm::vec3 PlanerNormal(Planer *planer) {
    glm::vec3 u = planar_points[planer->a] - planar_points[planer->c];
    glm::vec3 v = planar_points[planer->b] - planar_points[planer->c];
    return glm::cross(u, v);
}
*/

}  // namespace raychess
