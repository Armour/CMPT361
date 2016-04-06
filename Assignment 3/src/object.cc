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
#include "octree.h"
#include <iostream>

extern raychess::OctreeNode *octree;
extern int octree_on;

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
//       object_ignore: the sphere index that we need to ignore
//
//   Returns:
//       A pointer to the sphere object that the ray intersects first, nullptr if no intersection.
//

Object *IntersectScene(glm::vec3 origin, glm::vec3 direction, Object *objects, glm::vec3 *hit, int object_ignore) {
    Object *result = nullptr;
    glm::vec3 *current_hit = new glm::vec3();

    if (octree_on) {          // Ray intersection test with octree
        std::vector<OctreeNode *> nodes;
        raychess::RayTraverse(octree, origin, direction, nodes);

        for (auto node : nodes) {
            for (auto object : node->objects_) {
                if (object->get_index() != object_ignore) {
                    float distance = -1;
                    if (object->get_type() == libconsts::kTypeSphere)
                        distance = ((Sphere *) object)->IntersectRay(origin, direction, current_hit);
                    else if (object->get_type() == libconsts::kTypeTriangle)
                        distance = ((Triangle *) object)->IntersectRay(origin, direction, current_hit);

                    if (distance != -1 && distance < libconsts::kMaxDistance) {
                        if (result == nullptr || glm::length(*hit - origin) > glm::length(*current_hit - origin)) {
                            *hit = *current_hit;
                            result = object;
                        }
                    }
                }
            }
        }
    } else {                // Do ray intersection test without octree
        Object *current_object = objects;
        while (current_object != nullptr) {
            if (current_object->get_index() != object_ignore) {
                float distance = -1;
                if (current_object->get_type() == libconsts::kTypeSphere)
                    distance = ((Sphere *) current_object)->IntersectRay(origin, direction, current_hit);
                else if (current_object->get_type() == libconsts::kTypeTriangle)
                    distance = ((Triangle *) current_object)->IntersectRay(origin, direction, current_hit);

                if (distance != -1 && distance < libconsts::kMaxDistance) {
                    if (result == nullptr || glm::length(*hit - origin) > glm::length(*current_hit - origin)) {
                        *hit = *current_hit;
                        result = current_object;
                    }
                }
            }
            current_object = current_object->get_next();
        }
    }

    delete current_hit;
    return result;
}

}  // namespace raychess
