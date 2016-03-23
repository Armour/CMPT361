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

extern GLfloat frame[libconsts::kWindowSizeHeight][libconsts::kWindowSizeWidth][3];
extern raychess::Sphere *scene;
extern glm::vec3 background_color;
extern glm::vec3 light;
extern float light_intensity[3];
extern float global_ambient[3];
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

glm::vec3 PhongIllumination(glm::vec3 q, glm::vec3 v, glm::vec3 surf_norm, raychess::Sphere *sphere) {
    // TODO: add phong illumination
    glm::vec3 color;
    return color;
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

glm::vec3 RecursiveRayTrace() {
    // TODO: recursive here
    glm::vec3 color;
    return color;
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

            // TODO: finish recursive function
            // ret_color = RecursiveRayTrace();
            ret_color = background_color; // just background for now

            // Parallel rays can be cast instead using below
            //
            // ray.x = 0.0f;
            // ray.y = 0.0f;
            // ray.z = -1.0f;
            // ret_color = RecursiveRayTrace(cur_pixel_pos, ray, 1);

            glm::vec3 color = {float(i / 32), 0, float(j / 32)};
            ret_color = color;

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
