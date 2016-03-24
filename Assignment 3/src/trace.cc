//////////////////////////////////////////////////////////////////////////////
//
//  trace.cc
//
//  The source file for ray trace function
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

#include "trace.h"
#include <iostream>

extern GLfloat frame[libconsts::kWindowSizeHeight][libconsts::kWindowSizeWidth][3];
extern raychess::Sphere *scene;
extern glm::vec3 background_color;
extern glm::vec3 light;
extern glm::vec3 light_ambient;
extern glm::vec3 light_diffuse;
extern glm::vec3 light_specular;
extern glm::vec3 global_ambient;
extern float decay_a;
extern float decay_b;
extern float decay_c;
extern int shadow_on;
extern int step_max;

namespace raychess {

//
// Function: PhongIllumination
// ---------------------------
//
//   The phong illumination implementation
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

glm::vec3 PhongIllumination(raychess::Sphere *sphere, glm::vec3 hit, glm::vec3 surf_norm) {
    glm::vec3 intensity(0.0f);
    glm::vec3 *dummy = new glm::vec3();
    if (!shadow_on || IntersectScene(hit, light - hit, scene, dummy, sphere->index) == nullptr) {
        glm::vec3 l = glm::normalize(light - hit);
        glm::vec3 v = glm::normalize(libconsts::kEyePosition - hit);
        glm::vec3 r = 2.0f * surf_norm * (glm::dot(surf_norm, l)) - l;
        float diffuse = fmaxf(glm::dot(surf_norm, l), 0.0f);
        float specular = powf(fmaxf(glm::dot(v, r), 0.0f), sphere->mat_shininess);
        float delta = glm::length(light - hit);
        intensity = (light_ambient * sphere->mat_ambient +
                     light_diffuse * sphere->mat_diffuse * diffuse +
                     light_specular * sphere->mat_specular * specular) /
                    (decay_a + decay_b * delta + decay_c * delta * delta);
    }
    intensity += global_ambient * sphere->mat_ambient;
    delete dummy;
    return intensity;
}

//
// Function: RecursiveRayTrace
// ---------------------------
//
//   This is the recursive ray tracer
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

glm::vec3 RecursiveRayTrace(glm::vec3 origin, glm::vec3 direction, int iteration, int sphere_ignore) {
    if (iteration == 0) {
        return background_color;
    }
    glm::vec3 *hit = new glm::vec3();
    raychess::Sphere *sphere = IntersectScene(origin, direction, scene, hit, sphere_ignore);
    if (sphere != nullptr) {
        glm::vec3 surf_norm = SphereNormal(*hit, sphere);
        glm::vec3 color = PhongIllumination(sphere, *hit, surf_norm);
        return color;
    }
    return background_color;
}

//
// Function: RayTrace
// ---------------------------
//
//   This function traverses all the pixels and cast rays. It calls the recursive ray tracer and assign return color to frame
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void RayTrace() {
    int i, j;
    float x_grid_size = libconsts::kImageWidth / float(libconsts::kWindowSizeWidth);
    float y_grid_size = libconsts::kImageHeight / float(libconsts::kWindowSizeHeight);
    float x_start = -0.5f * libconsts::kImageWidth;
    float y_start = -0.5f * libconsts::kImageHeight;
    glm::vec3 ray;
    glm::vec3 ret_color;
    glm::vec3 cur_pixel_pos;

    // Ray is cast through center of pixel
    cur_pixel_pos.x = x_start + 0.5f * x_grid_size;
    cur_pixel_pos.y = y_start + 0.5f * y_grid_size;
    cur_pixel_pos.z = libconsts::kImagePlanePosZ;

    for (i = 0; i < libconsts::kWindowSizeHeight; i++) {
        for (j = 0; j < libconsts::kWindowSizeWidth; j++) {
            ray = cur_pixel_pos - libconsts::kEyePosition;

            ret_color = RecursiveRayTrace(cur_pixel_pos, ray, 1, 0);

            frame[i][j][0] = ret_color.r;
            frame[i][j][1] = ret_color.g;
            frame[i][j][2] = ret_color.b;

            cur_pixel_pos.x += x_grid_size;
        }
        cur_pixel_pos.y += y_grid_size;
        cur_pixel_pos.x = x_start;
    }
}

}  // namespace raychess
