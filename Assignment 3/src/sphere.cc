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

float IntersectSphere(glm::vec3 o, glm::vec3 u, raychess::Sphere *sphere, glm::vec3 *hit) {
    // TODO: finish intersect function
    return 0.0;
}

//
// Function: IntersectScene
// ---------------------------
//
//   This function returns a pointer to the sphere object that the
//   ray intersects first; NULL if no intersection. You should decide
//   which arguments to use for the function. For example, note that you
//   should return the point of intersection to the calling function.
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

raychess::Sphere *IntersectScene(glm::vec3, glm::vec3, Sphere *sphere, glm::vec3 *, int) {
    // TODO: finish intersect function
    return NULL;
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

raychess::Sphere *AddSphere(raychess::Sphere *spheres, glm::vec3 center, float radius, float ambient[],
                            float diffuse[], float specular[], float shineness, float reflectance, int index) {
    raychess::Sphere *new_sphere;
    new_sphere = (raychess::Sphere *)malloc(sizeof(raychess::Sphere));
    new_sphere->index = index;
    new_sphere->center = center;
    new_sphere->radius = radius;
    (new_sphere->mat_ambient)[0] = ambient[0];
    (new_sphere->mat_ambient)[1] = ambient[1];
    (new_sphere->mat_ambient)[2] = ambient[2];
    (new_sphere->mat_diffuse)[0] = diffuse[0];
    (new_sphere->mat_diffuse)[1] = diffuse[1];
    (new_sphere->mat_diffuse)[2] = diffuse[2];
    (new_sphere->mat_specular)[0] = specular[0];
    (new_sphere->mat_specular)[1] = specular[1];
    (new_sphere->mat_specular)[2] = specular[2];
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

}  // namespace raychess
