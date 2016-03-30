//////////////////////////////////////////////////////////////////////////////
//
//  object.h
//
//  The source file for object class
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

#include "object.h"
#include "sphere.h"
#include "triangle.h"

namespace raychess {

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
        if (current_object->get_index() != sphere_ignore) {
            float distance = -1;
            if (current_object->get_type() == libconsts::kTypeSphere)
                distance = ((Sphere *)current_object)->IntersectRay(origin, direction, current_hit);
            else if (current_object->get_type() == libconsts::kTypeTriangle)
                distance = ((Triangle *)current_object)->IntersectRay(origin, direction, current_hit);

            if (distance != -1 && distance < libconsts::kMaxDistance) {
                if (result == nullptr || glm::length(*hit - origin) > glm::length(*current_hit - origin)) {
                    *hit = *current_hit;
                    result = current_object;
                }
            }
        }
        current_object = current_object->get_next();
    }
    delete current_hit;
    return result;
}

}  // namespace raychess
