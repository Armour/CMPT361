//////////////////////////////////////////////////////////////////////////////
//
//  sphere.h
//
//  The header file to handle spheres
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

#ifndef RAYCHESS_SPHERE_H_
#define RAYCHESS_SPHERE_H_

#include "sphere.h"
#include "lib_gl.h"

namespace raychess {

typedef struct sphere {
    int index;                  // Identifies a sphere; must be greater than 0
    float radius;               // The radius of this sphere
    glm::vec3 center;           // The center point of this sphere
    float mat_ambient[3];       // Material property used in Phong model
    float mat_diffuse[3];
    float mat_specular[3];
    float mat_shininess;
    float reflectance;          // Determines how much reflected light contributes to the color of a pixel
    struct sphere *next;        // A list of spheres
} Sphere;

// Intersect ray with sphere
float IntersectSphere(glm::vec3 o, glm::vec3 u, raychess::Sphere *sphere, glm::vec3 *hit);

// Intersect ray with scene
raychess::Sphere *IntersectScene(glm::vec3, glm::vec3, Sphere *sphere, glm::vec3 *, int);

// Add a sphere to the sphere list
raychess::Sphere *AddSphere(raychess::Sphere *spheres, glm::vec3 center, float radius, float ambient[],
                            float diffuse[], float specular[], float shininess, float reflectance, int index);

// Return the unit normal at a point on sphere
glm::vec3 SphereNormal(glm::vec3 surf_point, raychess::Sphere *sphere);

}  // namespace raychess

#endif  // RAYCHESS_SPHERE_H_
